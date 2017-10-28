#ifndef KERNEL_PIT_H
#define KERNEL_PIT_H

#include <inlow/kernel/timer.h>

namespace Pit
{
	void initialize();
	void deregisterTimer(size_t index);
	size_t registerTimer(Timer* timer);
}

#endif
