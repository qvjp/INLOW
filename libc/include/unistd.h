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

#include <inlow/conf.h>
#if __USE_INLOW
#include <inlow/fork.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

#define F_OK 0
#define R_OK (1 << 0)
#define W_OK (1 << 1)
#define X_OK (1 << 2)

		extern char* optarg;
		extern int opterr;
		extern int optind;
		extern int optopt;

		int access(const char*, int);
		int close(int);
		int chdir(const char*);
		size_t confstr(int, char*, size_t);
		int execl(const char*, const char*, ...);
		int execv(const char*, char* const[]);
		int execve(const char*, char* const[], char* const[]);
		__noreturn void _exit(int);
		pid_t fork(void);
		int getopt(int, char* const[], const char*);
		ssize_t read(int, void*, size_t);
		unsigned int sleep(unsigned int);
		ssize_t write(int, const void*, size_t);

#if __USE_INLOW
		int fchdirat(int, const char*);
		pid_t rfork(int);
		pid_t regfork(int, struct regfork*);
#endif

		int execvp(const char*, char* const[]);

#ifdef __cplusplus
}
#endif
#endif
