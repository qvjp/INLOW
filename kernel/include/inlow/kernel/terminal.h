#ifndef KERNEL_TERMINAL_H
#define KERNEL_TERMINAL_H

#include <inlow/kernel/vnode.h>

class Terminal : public Vnode
{
	public:
			virtual ssize_t write(const void* buffer, size_t size);
};

extern Terminal terminal;

#endif
