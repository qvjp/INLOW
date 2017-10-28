#ifndef KERNEN_TIMER_H
#define KERNEL_TIMER_H

#include <inlow/timespec.h>
#include <inlow/kernel/kernel.h>

class Timer
{
	public:
			Timer(struct timespec time);
			void advance(unsigned long nanosecodes);
			void start();
			void wait();
	private:
			struct timespec time;
			size_t index;
};

#endif
