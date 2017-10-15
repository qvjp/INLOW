#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <inlow/syscall.h>
#include <inlow/kernel/kernel.h>

namespace Syscall
{
	NORETURN void exit(int status);
	ssize_t read(int fd, void* buffer, size_t size);
	ssize_t write(int fd, const void* buffer, size_t size);
	void badSyscall();
}

#endif
