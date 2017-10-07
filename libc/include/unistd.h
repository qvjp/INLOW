#ifndef _UNISTD_H
#define _UNISTD_H

#include <sys/cdefs.h>
#define __need_pid_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C"
{
#endif
		__noreturn void _exit(int);
		pid_t fork(void);
		int execv(const char*, char* const[]);
		int execve(const char*, char* const[], char* const[]);
		int execvp(const char*, char* const[]);
#ifdef __cplusplus
}
#endif
#endif
