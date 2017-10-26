#include <errno.h>
#include <inlow/kernel/vnode.h>

Vnode::Vnode(mode_t mode)
{
	this->mode = mode;
}

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

int Vnode::stat(struct stat* result)
{
	result->st_mode = mode;
	return 0;
}

ssize_t Vnode::write(const void*, size_t)
{
	errno = EBADF;
	return -1;
}
