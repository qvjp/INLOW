#ifndef KERNEL_FILE_H__
#define KERNEL_FILE_H__

#include <inlow/kernel/vnode.h>

class FileVnode : public Vnode {
public:
    FileVnode();
    virtual bool isSeekable();
    virtual ssize_t pread(void* buffer, size_t size, off_t offset);
private:
    const char* data;
    size_t fileSize;
};

#endif