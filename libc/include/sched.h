#ifndef _SCHED_H
#define _SCHED_H

#include <sys/cdefs.h>
#include <inlow/timespec.h>

#ifdef __cplusplus
extern "C"
{
#endif

		int sched_yield(void);

#ifdef __cplusplus
}
#endif

#endif
