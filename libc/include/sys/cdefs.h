#ifndef _SYS_CDEFS_H
#define _SYS_CDEFS_H

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
# define __noreturn _Noreturn

#else
#define __noreturn __attribute__((__noreturn__))
#endif


#endif
