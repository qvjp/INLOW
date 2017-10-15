#ifndef KERNEL_TERMINAL_H
#define KERNEL_TERMINAL_H

#include <inlow/kernel/keyboard.h>
#include <inlow/kernel/vnode.h>

class Terminal : public Vnode, public KeyboardListener
{
	public:
			Terminal();
			virtual ssize_t read(void* buffer, size_t size);
			virtual ssize_t write(const void* buffer, size_t size);
	private:
			virtual void onKeyboardEvent(int key);
			void writeToCircularBuffer(char c);
			char readFromCircularBuffer();
	private:
			char circularBuffer[4096];
			volatile size_t readIndex;
			volatile size_t writeIndex;
};

extern Terminal terminal;

#endif
