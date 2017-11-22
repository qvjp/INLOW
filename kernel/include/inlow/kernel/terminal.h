#ifndef KERNEL_TERMINAL_H
#define KERNEL_TERMINAL_H

#include <inlow/termios.h>
#include <inlow/kernel/keyboard.h>
#include <inlow/kernel/kthread.h>
#include <inlow/kernel/vnode.h>

#define TERMINAL_BUFFER_SIZE 4096

class TerminalBuffer
{
	public:
			TerminalBuffer();
			size_t available();
			bool backspace();
			void endLine();
			bool hasIncompleteLine();
			char read();
			void reset();
			void write(char c);
	private:
			char circularBuffer[TERMINAL_BUFFER_SIZE];
			size_t readIndex;
			size_t lineIndex;
			size_t writeIndex;
};

class Terminal : public Vnode, public KeyboardListener
{
	public:
			Terminal();
			virtual ssize_t read(void* buffer, size_t size);
			virtual int tcgetattr(struct termios* result);
			virtual int tcsetattr(int flags, const struct termios* termios);
			virtual ssize_t write(const void* buffer, size_t size);
	private:
			void handleCharacter(char c);
			void handleSequence(const char* sequence);
			virtual void onKeyboardEvent(int key);
	private:
			TerminalBuffer terminalBuffer;
			struct termios termio;
			unsigned int numEof;
			kthread_mutex_t mutex;
};

extern Terminal terminal;

#endif
