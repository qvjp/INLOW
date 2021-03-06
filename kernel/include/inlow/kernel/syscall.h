#ifndef KERNEL_SYSCALL_H
#define KERNEL_SYSCALL_H

#include <sys/types.h>
#include <inlow/fork.h>
#include <inlow/syscall.h>
#include <inlow/timespec.h>
#include <inlow/kernel/kernel.h>

struct __mmapRequest;
struct stat;
namespace Syscall
{
	NORETURN void abort();
	int clock_gettime(clockid_t clockid, struct timespec* result);
	int clock_nanosleep(clockid_t clockid, int flags, const struct timespec* requested, struct timespec* remaining);
	int close(int fd);
	size_t confstr(int name, char* buffer, size_t size);
	int execve(const char* path, char* const argv[], char* const envp[]);
	NORETURN void exit(int status);
	int fchdirat(int fd, const char* path);
	int fstat(int fd, struct stat* result);
	int fstatat(int fd, const char* restrict path, struct stat* restrict result, int flags);
	pid_t getpid();
	int kill(pid_t pid, int signal);
	int linkat(int oldFd, const char* oldPath, int newFd, const char* newPath, int flags);
	int mkdirat(int fd, const char* path, mode_t mode);
	void* mmap(__mmapRequest* request);
	int munmap(void* addr, size_t size);
	int openat(int fd, const char* path, int flags, mode_t mode);
	ssize_t read(int fd, void* buffer, size_t size);
	ssize_t readdir(int fd, unsigned long offset, void* buffer, size_t size);
	int renameat(int oldFd, const char* oldPath, int newFd, const char* newPath);
	pid_t regfork(int flags, struct regfork* registers);
	int sigaction(int signal, const struct sigaction* restrict action, struct sigaction* restrict old);
	int symlinkat(const char* targetPath, int fd, const char* linkPath);
	int tcgetattr(int fd, struct termios* result);
	int tcsetattr(int fd, int flags, const struct termios* termio);
	int unlinkat(int fd, const char* path, int flags);
	pid_t waitpid(pid_t pid, int* status, int flags);
	ssize_t write(int fd, const void* buffer, size_t size);
	
	void badSyscall();
}

#endif
