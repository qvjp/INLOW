#ifndef UNISTD_H_
#define UNISTD_H_

#include <sys/cdefs.h>
#define __need_pid_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C" {
#endif

__dead void _exit(int);

/**
 * 以下函数未实现，声明到此仅为编译all-target-libgcc通过
 */
pid_t fork(void);
int execv(const char*, char* const[]);
int execve(const char*, char* const[], char* const[]);
int execvp(const char*, char* const[]);


#ifdef __cplusplus
}
#endif


#endif
