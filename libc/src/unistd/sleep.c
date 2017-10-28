#include <time.h>
#include <unistd.h>

unsigned int sleep(unsigned int seconds)
{
	struct timespec requested;
	struct timespec remaining;

	requested.tv_sec = seconds;
	requested.tv_nsec = 0;

	int result = nanosleep(&requested, &remaining);
	if (result < 0)
			return remaining.tv_sec;
	return 0;
}
