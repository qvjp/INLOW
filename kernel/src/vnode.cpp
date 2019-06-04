#include <errno.h>
#include <inlow/kernel/vnode.h>


bool Vnode::isSeekable() {
    return false;
}

Vnode* Vnode::openat(const char* /*path*/, int /*flags*/, mode_t /*mode*/) {
    errno = ENOTDIR;
    return nullptr;
}

ssize_t Vnode::pread(void* /*buffer*/, size_t /*size*/, off_t /*offset*/) {
    errno = EBADF;
    return -1;
}

ssize_t Vnode::read(void* /*buffer*/, size_t /*size*/) {
    errno = EBADF;
    return -1;
}

ssize_t Vnode::write(const void* /*buffer*/, size_t /*size*/) {
    errno = EBADF;
    return -1;
}