#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <inlow/kernel/vnode.h>

static ino_t nextIno = 0;

Vnode::Vnode(mode_t mode, dev_t dev, ino_t ino)
{
	this->mode = mode;
	this->dev = dev;
	this->ino = ino;

	if (!ino)
	{
		this->ino = nextIno++;
	}
}

Reference<Vnode> resolvePath(const Reference<Vnode>& vnode, const char* path)
{
	if (!*path)
	{
		errno = ENOENT;
		return nullptr;
	}

	Reference<Vnode> currentVnode = vnode;
	char* pathCopy = strdup(path);
	if (!pathCopy)
	{
		errno = ENFILE;
		return nullptr;
	}

	char* currentName = pathCopy;
	char* slash = strchr(currentName, '/');

	while (slash)
	{
		*slash = '\0';
		if (*currentName)
		{
			currentVnode = currentVnode->getChildNode(currentName);
			if (!currentVnode)
			{
				free(pathCopy);
				return nullptr;
			}
			if (!S_ISDIR(currentVnode->mode))
			{
				free(pathCopy);
				errno = ENOTDIR;
				return nullptr;
			}
		}
		currentName = slash + 1;
		slash = strchr(currentName, '/');
	}

	if (*currentName)
	{
		currentVnode = currentVnode->getChildNode(currentName);
	}

	free(pathCopy);
	return currentVnode;
}

int Vnode::ftruncate(off_t)
{
	errno = EBADF;
	return -1;
}

Reference<Vnode> Vnode::getChildNode(const char*)
{
	errno = EBADF;
	return nullptr;
}

bool Vnode::isSeekable()
{
	return false;
}

int Vnode::link(const char*, const Reference<Vnode>&)
{
	errno = ENOTDIR;
	return -1;
}

int Vnode::mkdir(const char*, mode_t)
{
	errno = ENOTDIR;
	return -1;
}

bool Vnode::onUnlink()
{
	return true;
}

ssize_t Vnode::pread(void*, size_t, off_t)
{
	errno = EBADF;
	return -1;
}

ssize_t Vnode::pwrite(const void*, size_t, off_t)
{
	errno = EBADF;
	return -1;
}

ssize_t Vnode::read(void*, size_t)
{
	errno = EBADF;
	return -1;
}

ssize_t Vnode::readdir(unsigned long, void*, size_t)
{
	errno = EBADF;
	return -1;
}

int Vnode::rename(Reference<Vnode>&, const char*, const char*)
{
	errno = EBADF;
	return -1;
}

int Vnode::stat(struct stat* result)
{
	result->st_dev = dev;
	result->st_ino = ino;
	result->st_mode = mode;
	return 0;
}

int Vnode::tcgetattr(struct termios*)
{
	errno = ENOTTY;
	return -1;
}

int Vnode::tcsetattr(int, const struct termios*)
{
	errno = ENOTTY;
	return -1;
}

int Vnode::unlink(const char*, int)
{
	errno = ENOTDIR;
	return -1;
}

ssize_t Vnode::write(const void*, size_t)
{
	errno = EBADF;
	return -1;
}
