#ifndef _FCNTL_H
#define _FCNTL_H

#include <sys/cdefs.h>
#define __need_mode_t
#define __need_off_t
#define __need_pid_t
#include <sys/libc-types.h>
#include <sys/stat.h>
#include  <inlow/fcntl.h>

#ifdef __cplusplus
extern "C"
{
#endif
		int open(const char*, int, ...);
		int openat(int, const char*, int, ...);

#ifdef __cplusplus
}
#endif

#endif
