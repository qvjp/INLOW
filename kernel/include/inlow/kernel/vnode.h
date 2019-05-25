#ifndef KERNEL_VNODE_H__
#define KERNEL_VNODE_H__

#include <sys/types.h>

class Vnode {
public:
    virtual ssize_t read(void* buffer, size_t size);
    virtual ssize_t write(const void* buffer, size_t size);
    virtual ~Vnode() {}
};

#endif