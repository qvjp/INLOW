#ifndef KERNEL_TERMINAL_H
#define KERNEL_TERMINAL_H

#include <inlow/kernel/keyboard.h>
#include <inlow/kernel/vnode.h>

#define TERMINAL_BUFFER_SIZE 4096

class TerminalBuffer
{
	public:
			TerminalBuffer();
			bool backspace();
			char read();
			void write(char c);
	private:
			char circularBuffer[TERMINAL_BUFFER_SIZE];
			volatile size_t readIndex;
			volatile size_t lineIndex;
			volatile size_t writeIndex;
};

class Terminal : public Vnode, public KeyboardListener
{
	public:
			Terminal();
			virtual ssize_t read(void* buffer, size_t size);
			virtual ssize_t write(const void* buffer, size_t size);
	private:
			virtual void onKeyboardEvent(int key);
	private:
			TerminalBuffer terminalBuffer;
};

extern Terminal terminal;

#endif
