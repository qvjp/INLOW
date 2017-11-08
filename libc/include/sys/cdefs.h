#ifndef _SYS_CDEFS_H
#define _SYS_CDEFS_H

#include <features.h>

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
# define __noreturn _Noreturn

#else
#define __noreturn __attribute__((__noreturn__))
#endif

#define __printf_like(format, firstArg) \
		__attribute__((__format__(__printf__, format, firstArg)))


#endif
