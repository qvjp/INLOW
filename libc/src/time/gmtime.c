#include <time.h>

static struct tm buffer;

struct tm* gmtime(const time_t* time)
{
	return gmtime_r(time, &buffer);
}
