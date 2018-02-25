#ifndef KERNEL_CLOCK_H
#define KERNEL_CLOCK_H

#include <time.h>

class Clock
{
	public:
		Clock();
		int getTime(struct timespec* result);
		int nanosleep(int flags, const struct timespec* requested, struct timespec* remaining);
		int setTime(struct timespec* newValue);
		void tick(unsigned long nanoseconds);
	public:
		static Clock* get(clockid_t clockid);
		static void onTick(unsigned long nanoseconds);
	private:
		struct timespec value;
};

#endif
