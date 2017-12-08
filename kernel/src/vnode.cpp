#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <inlow/kernel/process.h>
#include <inlow/kernel/vnode.h>

#define SYMLOOP_MAX 20

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

static Reference<Vnode> resolvePathExceptLastComponent(const Reference<Vnode>& vnode,
		char* path, size_t& symlinksFollowed, char*& lastComponent, bool& endsWithSlash);

static Reference<Vnode> followPath(Reference<Vnode>& vnode, char* name,size_t& symlinksFollowed,
		bool followSymlink)
{
	Reference<Vnode> currentVnode = vnode;	
	Reference<Vnode> nextVnode = currentVnode->getChildNode(name);
	if (!nextVnode)
		return nullptr;

	while(S_ISLNK(nextVnode->mode) && followSymlink)
	{
		if (++symlinksFollowed > SYMLOOP_MAX)
		{
			errno = ELOOP;
			return nullptr;
		}
		char* symlinkDestination = nextVnode->getLinkTarget();
		if (!symlinkDestination)
			return nullptr;
		bool endsWithSlash;
		char* lastComponent;
		currentVnode = resolvePathExceptLastComponent(currentVnode,symlinkDestination,
				symlinksFollowed, lastComponent, endsWithSlash);
		if (!currentVnode)
		{
			free(symlinkDestination);
			return nullptr;
		}

		if (!*lastComponent)
		{
			free(symlinkDestination);
			return currentVnode;
		}

		nextVnode = currentVnode->getChildNode(lastComponent);
		free(symlinkDestination);
		if (!nextVnode)
			return nullptr;
	}
	return nextVnode;
}

static Reference<Vnode> resolvePathExceptLastComponent(const Reference<Vnode>& vnode,
		char* path, size_t& symlinksFollowed, char*& lastComponent, bool& endsWithSlash)
{
	Reference<Vnode> currentVnode = vnode;
	if (*path == '/')
	{
		currentVnode = Process::current->rootFd->vnode;
	}

	lastComponent = path;
	while (*lastComponent == '/')
	{
		lastComponent++;
	}
	char* slash = strchr(lastComponent, '/');

	while (slash)
	{
		*slash = '\0';
		char* next = slash + 1;
		while (*next == '/')
		{
			next++;
		}
		if (!*next)
			break;
		currentVnode = followPath(currentVnode, lastComponent, symlinksFollowed, true);
		if (!currentVnode)
			return nullptr;
		if (!S_ISDIR(currentVnode->mode))
		{
			errno = ENOTDIR;
			return nullptr;
		}
		lastComponent = next;
		slash = strchr(lastComponent, '/');
	}
	endsWithSlash = slash != nullptr;
	return currentVnode;
}
Reference<Vnode> resolvePathExceptLastComponent(const Reference<Vnode>& vnode,
		char* path, char** lastComponent)
{
	bool endsWithSlash;
	size_t symlinksFollowed = 0;
	return resolvePathExceptLastComponent(vnode, path, symlinksFollowed,
			*lastComponent, endsWithSlash);
}
Reference<Vnode> resolvePath(const Reference<Vnode>& vnode, const char* path, bool followFinalSymlink)
{
	if (!*path)
	{
		errno = ENOENT;
		return nullptr;
	}
	char* pathCopy = strdup(path);
	if (!pathCopy)
		return nullptr;

	char* lastComponent;
	bool endsWithSlash;
	size_t symlinksFollowed = 0;
	Reference<Vnode> currentVnode = resolvePathExceptLastComponent(vnode,
			pathCopy, symlinksFollowed, lastComponent, endsWithSlash);
	if (!currentVnode)
	{
		free(pathCopy);
		return nullptr;
	}
	if (!*lastComponent)
	{
		free(pathCopy);
		return currentVnode;
	}
	currentVnode = followPath(currentVnode, lastComponent, symlinksFollowed, followFinalSymlink);

	if (endsWithSlash && !S_ISDIR(currentVnode->mode))
	{
		errno = ENOTDIR;
		return nullptr;
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

char* Vnode::getLinkTarget()
{
	errno = EINVAL;
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
