#include <errno.h>
#include <time.h>

time_t mktime(struct tm* tm)
{
	tzset();
	time_t offset = tm->tm_isdst > 0 ? altzone : timezone;

	int oldErrno = errno;
	errno = 0;
	time_t result = timegm(tm);
	if (errno)
		return -1;

	errno = oldErrno;
	return result + offset;
}
