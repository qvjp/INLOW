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
 * kernel/src/arch/i686/interrrupt.s
 * 处理中断，保存现场调用ISRs恢复现场
 */

.section .text

/* 处理没有错误码的ISR */
.macro isr no
.global isr_\no
isr_\no:
    cli
    push $0 /* 对于没有错误码的异常，push一个0是为了统一栈帧 */
    push $\no
    jmp isr_commonHandler
.endm

/* 处理有错误码的ISR */
.macro isr_error_code no
.global isr_\no
isr_\no:
    cli
    /* 有错误码的isr就不需要手动push了，已经自动push进错误码了 */
    push $\no
    jmp isr_commonHandler
.endm

/**
 * 当ISR发生调用时，保存当前运行状态，
 * 设置内核模式段，调用C语言编写的错误处理程序，
 * 最后回到之前的栈帧
 */

isr_commonHandler:
    push %ebp
    push %edi
    push %esi
    push %edx
    push %ecx
    push %ebx
    push %eax

    # Switch to kernel data segment
    mov $0x10, %ax
    mov %ax, %ds

    mov %esp, %eax
    and $(~0xFF), %esp # Align the stack
    sub $12, %esp
    push %eax

    call interrupt_handler
    mov %eax, %esp

    # Switch back to user data segment
    mov $0x23, %ax
    mov %ax, %ds

    pop %eax
    pop %ebx
    pop %ecx
    pop %edx
    pop %esi
    pop %edi
    pop %ebp
    add $8, %esp
    iret            /* pop cs, eip, eflags, ss, esp */

/* Exception */
isr 0
isr 1
isr 2
isr 3
isr 4
isr 5
isr 6
isr 7
isr_error_code 8
isr 9
isr_error_code 10
isr_error_code 11
isr_error_code 12
isr_error_code 13
isr_error_code 14
isr 15
isr 16
isr 17
isr 18
isr 19
isr 20
isr 21
isr 22
isr 23
isr 24
isr 25
isr 26
isr 27
isr 28
isr 29
isr 30
isr 31

/* IRQs */
isr 32
isr 33
isr 34
isr 35
isr 36
isr 37
isr 38
isr 39
isr 40
isr 41
isr 42
isr 43
isr 44
isr 45
isr 46
isr 47

; isr 48 # Padding
isr 49 # Schedule

# isr 73 # Syscall
