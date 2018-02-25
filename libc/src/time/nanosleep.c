#include <time.h>

int nanosleep(const struct timespec* requested, struct timespec* remaining)
{
	int result = clock_nanosleep(CLOCK_REALTIME, 0, requested, remaining);
	return result ? -1 : 0;
}
