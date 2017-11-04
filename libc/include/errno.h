#ifndef _ERRNO_H
#define _ERRNO_H

#include <sys/cdefs.h>
#include <inlow/errno.h>

#if __USE_INLOW
extern char* program_invocation_name;
extern char* program_invocation_short_name;
#endif

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
