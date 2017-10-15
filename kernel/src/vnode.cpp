#include <errno.h>
#include <inlow/kernel/vnode.h>

ssize_t Vnode::read(void*, size_t)
{
	errno = ENOSYS;
	return -1;
}

ssize_t Vnode::write(const void*, size_t)
{
	errno = ENOSYS;
	return -1;
}
