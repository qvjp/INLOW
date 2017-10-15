#include <errno.h>
#include <inlow/kernel/vnode.h>

ssize_t Vnode::write(const void*, size_t)
{
	errno = ENOSYS;
	return -1;
}
