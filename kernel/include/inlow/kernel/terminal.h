#ifndef KERNEL_TERMINAL_H
#define KERNEL_TERMINAL_H

#include <inlow/kernel/keyboard.h>
#include <inlow/kernel/vnode.h>

class Terminal : public Vnode, public KeyboardListener
{
	public:
			virtual ssize_t write(const void* buffer, size_t size);
	private:
			virtual void onKeyboardEvent(int key);
};

extern Terminal terminal;

#endif
