#include <errno.h>
#include <inlow/kernel/clock.h>
#include <inlow/kernel/process.h>

static Clock monotonicClock;
static Clock realtimeClock;

Clock::Clock()
{
	value.tv_sec = 0;
	value.tv_nsec = 0;
}

Clock* Clock::get(clockid_t clockid)
{
	switch (clockid)
	{
		case CLOCK_MONOTONIC:
			return &monotonicClock;
		case CLOCK_REALTIME:
			return & realtimeClock;
		case CLOCK_PROCESS_CPUTIME_ID:
			return &Process::current->cpuClock;
		default:
			errno = EINVAL;
			return nullptr;
	}
}

int Clock::getTime(struct timespec* result)
{
	*result = value;
	return 0;
}

void Clock::tick(unsigned long nanoseconds)
{
	value.tv_nsec += nanoseconds;

	while (value.tv_nsec >= 1000000000L)
	{
		value.tv_sec++;
		value.tv_nsec -= 1000000000L;
	}
}

void Clock::onTick(unsigned long nanoseconds)
{
	monotonicClock.tick(nanoseconds);
	realtimeClock.tick(nanoseconds);
	Process::current->cpuClock.tick(nanoseconds);
}
