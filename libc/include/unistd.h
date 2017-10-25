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

#if __USE_INLOW
#include <inlow/fork.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif
		int close(int);
		__noreturn void _exit(int);
		pid_t fork(void);
		ssize_t read(int, void*, size_t);
		ssize_t write(int, const void*, size_t);

#if __USE_INLOW
		pid_t rfork(int);
		pid_t regfork(int, struct regfork*);
#endif

		int execv(const char*, char* const[]);
		int execve(const char*, char* const[], char* const[]);
		int execvp(const char*, char* const[]);

#ifdef __cplusplus
}
#endif
#endif
