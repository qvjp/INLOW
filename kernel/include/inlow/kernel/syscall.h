#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <inlow/syscall.h>
#include <inlow/kernel/kernel.h>

namespace Syscall
{
	void NORETURN exit(int status);
	void badSyscall();
}

#endif
