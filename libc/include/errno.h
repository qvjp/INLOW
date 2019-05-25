#ifndef ERRNO_H_
#define ERRNO_H_

#include <sys/cdefs.h>
#include <inlow/errno.h>
#ifdef __cplusplus
extern "C" {
#endif

extern int errno;
#define errno errno

#ifdef __cplusplus
}
#endif
#endif