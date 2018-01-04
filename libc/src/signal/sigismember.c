#include <errno.h>
#include <signal.h>

int sigismember(const sigset_t* set, int signal)
{
	if ((size_t) signal > sizeof(sigset_t) * 8)
	{
		errno = EINVAL;
		return -1;
	}
	return *set & _SIGSET(signal) ? 1 : 0;
}
