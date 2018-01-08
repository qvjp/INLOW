#include <signal.h>
#include <unistd.h>

int raise(int signal)
{
	return kill(getpid(), signal);
}
