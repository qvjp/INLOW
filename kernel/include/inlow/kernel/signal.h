#ifndef KERNEL_SIGNAL_H
#define KERNEL_SIGNAL_H

#include <inlow/kernel/interrupts.h>

extern "C" volatile unsigned long signalPending;

namespace Signal
{
	static inline bool isPending()
	{
		return signalPending;
	}
	InterruptContext* sigreturn(InterruptContext* context);
}

#endif
