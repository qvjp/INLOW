/** MIT License
 * 
 * Copyright (c) 2019 INLOW-OS
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * kernel/src/process.cpp
 * 实现INLOW中的进程
 */
#include <inlow/kernel/addressspace.h> /*  AddressSpace* fork(); */
#include <inlow/kernel/process.h>
#include <stdlib.h>                    /* malloc() */

static Process* currentProcess;
static Process* firstProcess;
static Process* idleProcess;

/**
 * 这里的进程我们只指一个程序的基本可执行实体，并不代表线程的容器（区别于现代面向线程设计的系统）。
 * 当前的INLOW中进程控制块比较简单，包括一个独立的地址空间、运行上下文、指向下一个进程的指针和内核堆栈、用户堆栈
 */
Process::Process()
{
    addressSpace = kernelSpace;
    interruptContext = nullptr;
    next = nullptr;
    stack = nullptr;
    kstack = nullptr;
}

/**
 * 进程初始化
 * 创建空闲进程（没有其他任务执行时执行的进程）
 */
void Process::initialize()
{
    idleProcess = (Process*)malloc(sizeof(Process));
    idleProcess->addressSpace = kernelSpace;
    idleProcess->next = 0;
    idleProcess->kstack = 0;
    idleProcess->stack = 0;
    idleProcess->interruptContext = (struct regs*)malloc(sizeof(struct regs));
    currentProcess = idleProcess;
    firstProcess = nullptr;
}

/**
 * 进程调度函数
 * 
 * 若当前正在执行的进程next域有其他进程，那么返回这个进程
 * 若没有其他进程，执行空闲进程
 */
struct regs* Process::schedule(struct regs* context)
{
    currentProcess->interruptContext = context;
    if (currentProcess->next)
    {
        currentProcess = currentProcess->next;
    }
    else
    {
        if (firstProcess)
        {
            currentProcess = firstProcess;
        }
        else
        {
            currentProcess = idleProcess;
        }
    }
    setKernelStack(currentProcess->kstack);

    currentProcess->addressSpace->activate();
    return currentProcess->interruptContext;
}

/**
 * 开始新进程
 * 传入地址空间是为能在调用出确定内核态或用户态
 * 最后返回新创建的进程
 */
Process* Process::startProcess(void* entry, AddressSpace* addressSpace)
{
    Process* process = (Process*)malloc(sizeof(Process));
    /**
     * 分配两个栈，内核栈用来保存上下文信息
     * 用户栈处理其他
     */
    process->kstack = (void*) kernelSpace->allocate(1);
    process->stack = (void*) addressSpace->allocate(1);

    process->interruptContext = (struct regs*)((uintptr_t) process->kstack + 0x1000 - sizeof(struct regs));
    process->interruptContext->eax = 0;
    process->interruptContext->ebx = 0;
    process->interruptContext->ecx = 0;
    process->interruptContext->edx = 0;
    process->interruptContext->esi = 0;
    process->interruptContext->edi = 0;
    process->interruptContext->ebp = 0;
    process->interruptContext->int_no = 0;
    process->interruptContext->err_code = 0;
    process->interruptContext->eip = (uint32_t)entry; 
    process->interruptContext->cs = 0x1B;         /* 用户代码段是0x18 因为在ring3， 
                                                   * 按Intel规定，要使用(0x18 | 0x3)
                                                   * 下边作为段选择子偏移ss的0x23也是
                                                   * 一样道理。
                                                   */
    process->interruptContext->eflags = 0x200;    /* 开启中断 */
    process->interruptContext->useresp = (uint32_t) process->stack + 0x1000;
    process->interruptContext->ss = 0x23;         /* 用户数据段 */

    process->addressSpace = addressSpace;

    process->next = firstProcess;
    firstProcess = process;
    return process;
}
