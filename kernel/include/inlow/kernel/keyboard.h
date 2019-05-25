#ifndef KERNEL_KEYBOARD_H__
#define KERNEL_KEYBOARD_H__

namespace Keyboard {
char getCharFromKey(int key);
}

class KeyboardListener {
public:
    virtual void onKeyboardEvent(int key) = 0;
    virtual ~KeyboardListener() {}
};

#endif