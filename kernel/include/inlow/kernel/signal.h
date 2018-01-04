#ifndef KERNEL_SIGNAL_H
#define KERNEL_SIGNAL_H

extern "C" volatile unsigned long signalPending;

namespace Signal
{
	static inline bool isPending()
	{
		return signalPending;
	}
}

#endif
