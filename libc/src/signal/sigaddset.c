#include <errno.h>
#include <signal.h>

int sigaddset(sigset_t* set, int signal)
{
	if ((size_t) signal > sizeof(sigset_t) * 8)
	{
		errno = EINVAL;
		return -1;
	}
	*set |= _SIGSET(signal);
	return 0;
}
