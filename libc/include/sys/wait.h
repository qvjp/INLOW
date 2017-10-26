#ifndef _SYS_WAIT_H
#define _SYS_WAIT_H

#include <sys/cdefs.h>
#define __need_id_t
#define __need_pid_t
#define __need_siginfo_t

#include <sys/libc-types.h>
#include <inlow/wait.h>

#ifdef __cplusplus
extern "C"
{
#endif
		pid_t waitpid(pid_t, int* ,int);
#ifdef __cplusplus
}
#endif

#endif
