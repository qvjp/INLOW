#ifndef _TIME_H
#define _TIME_H

#include <sys/cdefs.h>

#define __need_clock_t
#define __need_locale_t
#define __need_NULL
#define __need_size_t
#define __need_time_t
#if __USE_INLOW || _USE_POSIX
#define __need_clockid_t
#define __need_timer_t
#endif

#include <sys/libc-types.h>
#include <inlow/timespec.h>
#if __USE_INLOW || __USE_POSIX
#include <inlow/clock.h>
#endif

#ifdef __cplusplus
extern "C"
{
#endif

struct tm
{
	int tm_sec;
	int tm_min;
	int tm_hour;
	int tm_mday;
	int tm_mon;
	int tm_year;
	int tm_wday;
	int tm_yday;
	int tm_isdst;
};

time_t time(time_t*);
#if __USE_INLOW || __USE_POSIX
	int clock_gettime(clockid_t, struct timespec*);
	int clock_nanosleep(clockid_t, int, const struct timespec*, struct timespec*);
	int nanosleep(const struct timespec*, struct timespec*);
#endif

#if __USE_INLOW
	time_t timegm(struct tm*);
#endif

#ifdef __cplusplus
}
#endif

#endif
