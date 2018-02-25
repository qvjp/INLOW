#include <time.h>

long altzone = 0;
int daylight = 0;
long timezone = 0;
char* tzname[2] = { "UTC", "UTC" };

void tzset(void)
{
	// TODO: Set timezone based on $TZ
}
