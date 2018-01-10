#include <signal.h>

void (*signal(int signum, void (*handler)(int)))(int)
{
	struct sigaction action;
	action.sa_handler = handler;
	action.sa_flags = 0;
	sigemptyset(&action.sa_mask);

	struct sigaction old;

	if (sigaction(signum, &action, &old) < 0)
	{
		return SIG_ERR;
	}
	return old.sa_handler;
}
