#ifndef KERNEL_TERMINAL_H__
#define KERNEL_TERMINAL_H__

#include <inlow/kernel/keyboard.h>
#include <inlow/kernel/vnode.h>

class Terminal : public Vnode, public KeyboardListener {
public:
    virtual ssize_t write(const void* buffer, size_t size);
private:
    virtual void onKeyboardEvent(int key);
public:
    static void warnTerminal();
    static void initTerminal();
    
};

extern Terminal terminal;

#endif