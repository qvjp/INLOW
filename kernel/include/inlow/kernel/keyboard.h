#ifndef KERNEL_KEYBOARD_H
#define KERNEL_KEYBOARD_H

namespace Keyboard
{
	char getCharFromKey(int key);
	const char* getSequenceFromKey(int key);
}

class KeyboardListener
{
	public:
			virtual void onKeyboardEvent(int key) = 0;
			virtual ~KeyboardListener(){}
};

#endif
