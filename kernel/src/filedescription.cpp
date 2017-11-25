#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <inlow/fcntl.h>
#include <inlow/kernel/directory.h>
#include <inlow/kernel/file.h>
#include <inlow/kernel/filedescription.h>

FileDescription::FileDescription(Vnode* vnode)
{
	this->vnode = vnode;
	offset = 0;
}

FileDescription* FileDescription::openat(const char* path, int flags, mode_t mode)
{
	Vnode* node = resolvePath(vnode, path);
	if (!node)
	{
		if (!(flags & O_CREAT))
			return nullptr;	
		char* pathCopy = strdup(path);
		char* slash = strrchr(pathCopy, '/');
		char* newFileName;

		if (!slash || slash == pathCopy)
		{
			node = vnode;
			newFileName = slash ? pathCopy + 1 : pathCopy;
		}
		else
		{
			*slash = '\0';
			newFileName = slash + 1;
			node = resolvePath(vnode, pathCopy);
			if (!node)
			{
				free(pathCopy);
				return nullptr;
			}
		}
		if (!S_ISDIR(node->mode))
		{
			free(pathCopy);
			errno = ENOTDIR;
			return nullptr;
		}

		FileVnode* file = new FileVnode(nullptr, 0, mode & 07777, vnode->dev, 0);
		DirectoryVnode* directory = (DirectoryVnode*) node;
		if (!directory->addChildNode(newFileName, file))
		{
			free(pathCopy);
			delete file;
			return nullptr;
		}

		free(pathCopy);
		node = file;
	}
	if (flags & O_TRUNC)
		node->ftruncate(0);
	return new FileDescription(node);
}

ssize_t FileDescription::read(void* buffer, size_t size)
{
		if (vnode->isSeekable())
		{
			ssize_t result = vnode->pread(buffer, size, offset);

			if (result != -1)
					offset += result;
			return result;
		}
	return vnode->read(buffer, size);
}

ssize_t FileDescription::readdir(unsigned long offset, void* buffer, size_t size)
{
	return vnode->readdir(offset, buffer, size);
}

int FileDescription::tcgetattr(struct termios* result)
{
	return vnode->tcgetattr(result);
}

int FileDescription::tcsetattr(int flags, const struct termios* termio)
{
	return vnode->tcsetattr(flags, termio);
}

ssize_t FileDescription::write(const void* buffer, size_t size)
{
	if (vnode->isSeekable())
	{
		ssize_t result = vnode->pwrite(buffer, size, offset);
		if (result != -1)
		{
			offset += result;
		}
		return result;
	}
	return vnode->write(buffer, size);
}
