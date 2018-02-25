#include <time.h>

struct tm* localtime(const time_t* time)
{
	tzset();
	time_t local = *time - timezone;
	return gmtime(&local);
}
