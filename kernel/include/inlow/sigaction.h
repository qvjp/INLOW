#ifndef _INLOW_SIGACTION_H
#define _INLOW_SIGACTION_H

#include <inlow/siginfo.h>
#include <inlow/sigset.h>

struct sigaction
{
	sigset_t sa_mask;
	int sa_flags;
	union
	{
		void (*sa_handler)(int);
		void (*sa_sigaction)(int, siginfo_t*, void*);
	};
};

#define SA_SIGINFO (1 << 0)

#endif
