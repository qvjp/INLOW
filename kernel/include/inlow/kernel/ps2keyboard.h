#ifndef KERNEL_PS2KEYBOARD_H
#define KERNEL_PS2KEYBOARD_H

#include <inlow/kernel/keyboard.h>
#include <inlow/kernel/ps2.h>

class PS2Keyboard : public PS2Device
{
	public:
			PS2Keyboard();
			virtual void irqHandler();
	private:
			void handleKey(int keycode);
	public:
			KeyboardListener* listener;
};

#endif
