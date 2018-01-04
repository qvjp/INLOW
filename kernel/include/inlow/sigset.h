#ifndef _INLOW_SIGSET_H
#define _INLOW_SIGSET_H

typedef __UINT64_TYPE__ sigset_t;

#define _SIGSET(signal) (1 << ((signal) - 1))

#endif
