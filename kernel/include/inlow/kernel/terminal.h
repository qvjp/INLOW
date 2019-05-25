#ifndef KERNEL_TERMINAL_H__
#define KERNEL_TERMINAL_H__

#include <inlow/kernel/vnode.h>

class Terminal : public Vnode {
public:
    virtual ssize_t write(const void* buffer, size_t size);
    static void warnTerminal();
    static void initTerminal();
    
};

extern Terminal terminal;

#endif