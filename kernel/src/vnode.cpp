#include <errno.h>
#include <inlow/kernel/vnode.h>

ssize_t Vnode::read(void* /*buffer*/, size_t /*size*/) {
    errno = ENOSYS;
    return -1;
}

ssize_t Vnode::write(const void* /*buffer*/, size_t /*size*/) {
    errno = ENOSYS;
    return -1;
}