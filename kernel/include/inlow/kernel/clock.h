#ifndef KERNEL_CLOCK_H
#define KERNEL_CLOCK_H

#include <time.h>

class Clock
{
	public:
		Clock();
		void tick(unsigned long nanoseconds);
		int getTime(struct timespec* result);
	public:
		static Clock* get(clockid_t clockid);
		static void onTick(unsigned long nanoseconds);
	private:
		struct timespec value;
};

#endif
