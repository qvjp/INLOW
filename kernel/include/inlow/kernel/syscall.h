#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <sys/types.h>
#include <inlow/fork.h>
#include <inlow/syscall.h>
#include <inlow/kernel/kernel.h>

struct __mmapRequest;
struct stat;
namespace Syscall
{
	int close(int fd);
	int execve(const char* path, char* const argv[], char* const envp[]);
	NORETURN void exit(int status);
	int fstatat(int fd, const char* restrict path, struct stat* restrict result, int flags);
	void* mmap(__mmapRequest* request);
	int munmap(void* addr, size_t size);
	int openat(int fd, const char* path, int flags, mode_t mode);
	ssize_t read(int fd, void* buffer, size_t size);
	pid_t regfork(int flags, struct regfork* registers);
	pid_t waitpid(pid_t pid, int* status, int flags);
	ssize_t write(int fd, const void* buffer, size_t size);
	
	void badSyscall();
}

#endif
