#include <time.h>

time_t time(time_t* result)
{
	struct timespec ts;
	if (clock_gettime(CLOCK_REALTIME, &ts) < 0)
	{
		return -1;
	}

	if (result)
	{
		*result = ts.tv_sec;
	}

	return ts.tv_sec;
}
