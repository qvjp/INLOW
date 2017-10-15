#ifndef _ERRNO_H
#define _ERRNO_H

#include <sys/cdefs.h>
#include <inlow/errno.h>

#ifdef __cplusplus
extern "C"
{
#endif
		extern int errno;
#define errno errno
#ifdef __cplusplus
}
#endif

#endif
