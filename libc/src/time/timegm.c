#include <errno.h>
#include <stdbool.h>
#include <time.h>

static bool isLeapYear(time_t year)
{
	return !(year % 4) && ((year % 100) || !(year % 400));
}

static time_t daysPerYear(time_t year)
{
	return isLeapYear(year) ? 366 : 365;
}

enum
{
	JANUARY = 0,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER
};

static time_t daysPerMonth(int month, time_t year)
{
	switch (month)
	{
		case JANUARY:
			return 31;
		case FEBRUARY:
			return isLeapYear(year) ? 29 : 28;
		case MARCH:
			return 31;
		case APRIL:
			return 30;
		case MAY:
			return 31;
		case JUNE:
			return 30;
		case JULY:
			return 31;
		case AUGUST:
			return 31;
		case SEPTEMBER:
			return 30;
		case OCTOBER:
			return 31;
		case NOVEMBER:
			return 30;
		case DECEMBER:
			return 31;
		default:
			__builtin_unreachable();
	}
}

static bool normalize(int* x, int* y, int range)
{
	if (__builtin_add_overflow(*y, *x / range, y))
		return false;
	*x %= range;
	if (*x < 0)
	{
		*x += range;
		(*y)--;
	}

	return true;
}

static bool normalizeEntries(struct tm* tm)
{
	if (!normalize(&tm->tm_sec, &tm->tm_min, 60) ||
			!normalize(&tm->tm_min, &tm->tm_hour, 60) ||
			!normalize(&tm->tm_hour, &tm->tm_mday, 24))
	{
		return false;
	}

	if (!normalize(&tm->tm_mon, &tm->tm_year, 12))
		return false;
	
	while (tm->tm_mday > daysPerMonth(tm->tm_mon, (time_t) tm->tm_year + 1900))
	{
		tm->tm_mday -= daysPerMonth(tm->tm_mon, (time_t) tm->tm_year + 1900);
		tm->tm_mon++;
		if (tm->tm_mon > DECEMBER)
		{
			if (__builtin_add_overflow(tm->tm_year, 1, &tm->tm_year))
			{
				return false;
			}
			tm->tm_mon = JANUARY;
		}
	}
	while (tm->tm_mday <= 0)
	{
		tm->tm_mon--;
		if (tm->tm_mon < JANUARY)
		{
			if (__builtin_sub_overflow(tm->tm_year, 1, &tm->tm_year))
			{
				return false;
			}
			tm->tm_mon = DECEMBER;
		}
		tm->tm_mday += daysPerMonth(tm->tm_mon, (time_t) tm->tm_year + 1900);
	}
	return true;
}

time_t timegm(struct tm* tm)
{
	if (!normalizeEntries(tm))
	{
		errno = EOVERFLOW;
		return -1;
	}

	time_t year = 1970;
	time_t daysSinceEpoch = 0;
	while (year < 1900 + (time_t) tm->tm_year)
	{
		daysSinceEpoch += daysPerYear(year);
		year++;
	}

	while (year > 1900 + (time_t) tm->tm_year)
	{
		year--;
		daysSinceEpoch -= daysPerYear(year);
	}

	int month = JANUARY;
	tm->tm_yday = 0;

	while (month < tm->tm_mon)
	{
		daysSinceEpoch += daysPerMonth(month, year);
		tm->tm_yday += daysPerMonth(month, year);
		month++;
	}

	daysSinceEpoch += tm->tm_mday - 1;

	time_t secondsSinceEpoch = daysSinceEpoch * 24 * 60 * 60;
	tm->tm_yday += tm->tm_mday - 1;
	
	secondsSinceEpoch += tm->tm_hour * 60 * 60;
	secondsSinceEpoch += tm->tm_min * 60;
	secondsSinceEpoch += tm->tm_sec;

	tm->tm_wday = (4 + daysSinceEpoch) % 7;
	if (tm->tm_wday < 0)
	{
		tm->tm_wday += 7;
	}

	return secondsSinceEpoch;
}
