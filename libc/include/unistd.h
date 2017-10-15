#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/cdefs.h>
#define __need_gid_t
#define __need_NULL
#define __need_off_t
#define __need_pid_t
#define __need_size_t
#define __need_ssize_t
#define __uid_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C"
{
#endif
		__noreturn void _exit(int);
		ssize_t read(int, void*, size_t);
		ssize_t write(int, const void*, size_t);
		pid_t fork(void);
		int execv(const char*, char* const[]);
		int execve(const char*, char* const[], char* const[]);
		int execvp(const char*, char* const[]);
#ifdef __cplusplus
}
#endif
#endif
