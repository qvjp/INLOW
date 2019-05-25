#include <errno.h>
#include <inlow/kernel/vnode.h>

// Default implementation. Inheriting classes will override these functions.
ssize_t Vnode::write(const void* /*buffer*/, size_t /*size*/) {
    errno = ENOSYS;
    return -1;
}