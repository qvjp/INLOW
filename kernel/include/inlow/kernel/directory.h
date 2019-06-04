#ifndef KERNEL_DIRECTORY_H__
#define KERNEL_DIRECTORY_H__

#include <inlow/kernel/vnode.h>

class DirectoryVnode : public Vnode {
public:
    DirectoryVnode();
    void addChildNode(const char* path, Vnode* vnode);
    virtual Vnode* openat(const char* path, int flags, mode_t mode);
private:
    size_t childCount;
    Vnode** childNodes;
    const char** fileNames;
};

#endif