#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <sys/types.h>
#include <inlow/fork.h>
#include <inlow/syscall.h>
#include <inlow/kernel/kernel.h>

struct __mmapRequest;
namespace Syscall
{
	int execve(const char* path, char* const argv[], char* const envp[]);
	NORETURN void exit(int status);
	ssize_t read(int fd, void* buffer, size_t size);
	ssize_t write(int fd, const void* buffer, size_t size);
	void* mmap(__mmapRequest* request);
	int munmap(void* addr, size_t size);
	int openat(int fd, const char* path, int flags, mode_t mode);
	int close(int fd);
	pid_t regfork(int flags, struct regfork* registers);
	
	void badSyscall();
}

#endif
