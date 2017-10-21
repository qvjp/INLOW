#include <errno.h>
#include <inlow/kernel/vnode.h>

bool Vnode::isSeekable()
{
	return false;
}

Vnode* Vnode::openat(const char*, int, mode_t)
{
	errno = ENOTDIR;
	return nullptr;
}

ssize_t Vnode::pread(void*, size_t, off_t)
{
	errno = EBADF;
	return -1;
}

ssize_t Vnode::read(void*, size_t)
{
	errno = EBADF;
	return -1;
}

ssize_t Vnode::write(const void*, size_t)
{
	errno = EBADF;
	return -1;
}
