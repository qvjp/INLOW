#include <inlow/kernel/print.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/syscall.h>

/**
 * 系统调用表
 */
static const void* syscallList[NUM_SYSCALLS] =
{
    // (void*) Syscall::pad,
    
    (void*) Syscall::exit,
    (void*) Syscall::write,
};

/**
 * 通过系统调用号获取系统调用具体入口地址
 */
extern "C" const void* getSyscallHandler(unsigned interruptNum)
{
    if (interruptNum >= NUM_SYSCALLS)
        return (void*) Syscall::badSyscall;
    else
        return syscallList[interruptNum];
}

/**
 * 系统调用：pad()
 * 保留的0号系统调用
 */
// void Syscall::pad()
// {
//     const char* INLOW = "INLOW";
//     int num = 0;
//     for (int i = 0; i < 5; i++)
//     {
//         num +=  INLOW[i] - 'A' + 1;
//     }
//     Print::printf("Pad: %d\n", num);
// }

/**
 * 系统调用：exit()
 * 退出当前进程，通过调用当前进程的exit方法，并发起49号中断（调度中断）
 * 
 * 设置为__attribute__((__noreturn__))说明此函数不会返回，控制权
 * 不会再返回调用者，而最后的__builtin_unreachable()则告诉GCC程序
 * 不会执行到那里，否则声明了没有返回的函数GCC会发出警告，要注意的是函数
 * 返回值void不是不返回，是没有返回“值”，还是会返回给调用者。
 */
void __attribute__((__noreturn__)) Syscall::exit(int status)
{
    Process::current->exit(status);
    /**
     * 0x31 系统调度
     */
    __asm__ __volatile__ ("int $0x31");
    __builtin_unreachable();
}

ssize_t Syscall::write(int fd, const void* buffer, size_t size) {
    FileDescription* descr = Process::current->fd[fd];
    return descr->write(buffer, size);
}

/**
 * INLOW不能处理的系统调用
 */
void Syscall::badSyscall()
{
    Print::printf("Syscall::badSyscall was called\n");
}
