#ifndef _FEAtURES_H
#define _FEATURES_H

#define __inlow_libc__ 1

#if !defined(__STRICT_ANSI__) && \
		!defined(_ANSI_SOURCE) && \
		!defined(_ISOC99_SOURCE) && \
		!defined(_ISOC11_SOURCE) && \
		!defined(_POSIX_SOURCE) && \
		!defined(_POSIX_C_SOURCE) && \
		!defined(_INLOW_SOURCE) && \
		!defined(_ALL_SOURCE) && \
		!defined(_DEFAULT_SOURCE)
# define _DEFAULT_SOURCE 1
#endif

#if defined(_ALL_SOURCE)
#if !defined(_ISOC11_SOURCE)
define _ISOC11_SOURCE 1
#endif
#if !defined(_POSIX_C_SOURCE)
define _POSIX_C_SOURCE 200809L
#endif
#if !defined(_INLOW_SOURCE)
define _INLOW_SOURCE 1
#endif
#endif

#if defined(_DEFAULT_SOURCE) && !defined(_INLOW_SOURCE)
#define _INLOW_SOURCE 1
#endif

#if defined(_ISOC11_SOURCE)
#define __USE_C 2011
#elif defined(_ISOC99_SOURCE)
#define __USE_C 1999
#elif defined(_ANSI_SOURCE)
#define __USE_C 1989
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
#define __USE_C 2011
#elif (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L) || \
														(defined(__cplusplus) && __cplusplus >= 201103L)
#define __USE_C 1999
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199409L
#define __USE_C 1995
#else
#define __USE_C 1989
#endif

#if defined(_POSIX_C_SOURCE)
#if _POSIX_C_SOURCE >= 200809L
#define __USE_POSIX 200809L
#elif _POSIX_C_SOURCE >= 201112L
#define __USE_POSIX 200112L
#elif _POSIX_C_SOURCE >= 199506L
#define __USE_POSIX 199506L
#elif _POSIX_C_SOURCE >= 199309L
#define __USE_POSIX 199309L
#elif _POSIX_C_SOURCE >= 2
#define __USE_POSIX 199209L
#else
#define __USE_POSIX 199009L
#endif
#elif defined(_POSIX_SOURCE)
#define __USE_POSIX 198808L
#else
#define __USE_POSIX 0
#endif

#if __USE_POSIX >= 200112L && __USE_C < 1999
#undef __USE_C
#define __USE_C 1999
#endif

#if defined(_INLOW_SOURCE)
#define __USE_INLOW 1
#else
#define __USE_INLOW 0
#endif

#endif
