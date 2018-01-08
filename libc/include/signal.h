#ifndef _SIGNAL_H
#define _SIGNAL_H

#include <sys/cdefs.h>
#include <inlow/signals.h>
#if __USE_INLOW || __USE_POSIX
#define __need_pid_t
#define __need_pthread_t
#define __need_pthread_attr_t
#define __need_size_t
#define __need_uid_t
#include <sys/libc-types.h>
#include <inlow/siginfo.h>
#include <inlow/signalcodes.h>
#include <inlow/sigset.h>
#include <inlow/timespec.h>

#endif

#ifdef __cplusplus
extern "C"
{
#endif

	typedef __SIG_ATOMIC_TYPE__ sig_atomic_t;

	int raise(int);
#if __USE_INLOW || __USE_POSIX
	int kill(pid_t, int);
	int sigaddset(sigset_t*, int);
	int sigdelset(sigset_t*, int);
	int sigemptyset(sigset_t*);
	int sigfillset(sigset_t*);
	int sigismember(const sigset_t*, int);
#endif

#if __USE_INLOW
#define NSIG _NSIG
#endif


#ifdef __cplusplus
}
#endif

#endif
