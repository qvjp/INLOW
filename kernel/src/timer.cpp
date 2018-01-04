#include <errno.h>
#include <sched.h>
#include <inlow/kernel/pit.h>
#include <inlow/kernel/signal.h>
#include <inlow/kernel/syscall.h>


static inline void minus(struct timespec* time, unsigned long nanoseconds)
{
	time->tv_nsec -= nanoseconds;
	while (time->tv_nsec < 0)
	{
		time->tv_sec--;
		time->tv_nsec += 1000000000L;
	}
	if (time->tv_sec < 0)
	{
		time->tv_sec = 0;
		time->tv_nsec = 0;
	}
}

static inline bool isZero(struct timespec time)
{
	return (time.tv_sec == 0 && time.tv_nsec == 0);
}

Timer::Timer(struct timespec time)
{
	remaining = time;
	index = 0;
}

void Timer::advance(unsigned long nanoseconds)
{
	minus(&remaining, nanoseconds);
}

void Timer::start()
{
	index = Pit::registerTimer(this);
}

void Timer::wait()
{
	while (!isZero(remaining) && !Signal::isPending())
	{
		sched_yield();
	}
	Pit::deregisterTimer(index);
}

int Syscall::nanosleep(const timespec* requested, timespec* remaining)
{
	Timer timer(*requested);
	timer.start();
	timer.wait();

	if (remaining)
	{
		*remaining = timer.remaining;
	}

	if (!isZero(timer.remaining))
	{
		errno = EINTR;
		return -1;
	}

	return 0;
}
