#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <inlow/dirent.h>
#include <inlow/fcntl.h>
#include <inlow/kernel/directory.h>
#include <inlow/kernel/print.h>

DirectoryVnode::DirectoryVnode(const Reference<DirectoryVnode>& parent, mode_t mode,
				dev_t dev, ino_t ino) : Vnode(S_IFDIR | mode, dev, ino), parent(parent)
{
	childCount = 0;
	childNodes = nullptr;
	fileNames = nullptr;
	mutex = KTHREAD_MUTEX_INITIALIZER;
}

DirectoryVnode::~DirectoryVnode()
{
	free(childNodes);
	free(fileNames);
}

bool DirectoryVnode::addChildNode(const char* name, const Reference<Vnode>& vnode)
{
	AutoLock lock(&mutex);
	return addChildNodeUnlocked(name, vnode);
}

bool DirectoryVnode::addChildNodeUnlocked(const char* name, const Reference<Vnode>& vnode)
{
	Reference<Vnode>* newChildNodes = (Reference<Vnode>*) reallocarray(childNodes, childCount + 1, sizeof(Reference<Vnode>));
	if (!newChildNodes)
			return false;
	char** newFileNames = (char**) reallocarray(fileNames, childCount + 1, sizeof(const char*));
	if (!newFileNames)
			return false;
	childNodes = newChildNodes;
	fileNames = newFileNames;

	new (&childNodes[childCount]) Reference<Vnode>(vnode);
	fileNames[childCount] = strdup(name);
	childCount++;
	return true;
}

Reference<Vnode> DirectoryVnode::getChildNode(const char* name)
{
	AutoLock lock(&mutex);
	return getChildNodeUnlocked(name);
}

Reference<Vnode> DirectoryVnode::getChildNodeUnlocked(const char* name)
{
	if (strcmp(name, ".") == 0)
	{
		return this;
	}
	else if (strcmp(name, "..") == 0)
	{
		return parent ? parent : this;
	}
	for (size_t i = 0; i < childCount; i++)
	{
		if (strcmp(name, fileNames[i]) == 0)
		{
			return childNodes[i];
		}
	}

	errno = ENOENT;
	return nullptr;
}

int DirectoryVnode::mkdir(const char* name, mode_t mode)
{
	AutoLock lock(&mutex);

	if (!*name || getChildNodeUnlocked(name))
	{
		errno = EEXIST;
		return -1;
	}

	Reference<DirectoryVnode> newDirectory(new DirectoryVnode(this, mode, dev, 0));
	if (!addChildNodeUnlocked(name, newDirectory))
		return -1;
	return 0;
}

bool DirectoryVnode::onUnlink()
{
	if (childCount > 0)
	{
		errno = ENOTEMPTY;
		return false;
	}
	return true;
}

ssize_t DirectoryVnode::readdir(unsigned long offset, void* buffer, size_t size)
{
	AutoLock lock(&mutex);
	const char* name;
	Reference<Vnode> vnode;
	if (offset == 0)
	{
			name = ".";
			vnode = this;
	}
	else if (offset == 1)
	{
			name = "..";
			vnode = parent ? parent : this;
	}
	else if (offset - 2 < childCount)
	{
			name = fileNames[offset - 2];
			vnode = childNodes[offset - 2];
	}
	else if (offset - 2 == childCount)
			return 0;
	else
	{
		return -1;
	}

	size_t structSize = sizeof(struct dirent) + strlen(name) + 1;
	if (size >= structSize)
	{
		struct dirent* entry = (struct dirent*) buffer;
		entry->d_dev = vnode->dev;
		entry->d_ino = vnode->ino;
		entry->d_reclen = size;
		strcpy(entry->d_name, name);
	}

	return structSize;
}

int DirectoryVnode::rename(Reference<Vnode>& oldDirectory, const char* oldName, const char* newName)
{
	AutoLock lock(&mutex);
	Reference<Vnode> vnode;
	if (oldDirectory == this)
	{
		vnode = getChildNodeUnlocked(oldName);
	}
	else
	{
		vnode = oldDirectory->getChildNode(oldName);
	}
	if (!vnode)
		return -1;

	Reference<Vnode> vnode2 = getChildNodeUnlocked(newName);
	if (vnode == vnode2)
		return 0;

	for (size_t i = 0; i < childCount; i++)
	{
		if (strcmp(newName, fileNames[i]) == 0)
		{
			if (!S_ISDIR(vnode->mode) && S_ISDIR(childNodes[i]->mode))
			{
				errno = EISDIR;
				return -1;
			}
			if (S_ISDIR(vnode->mode) && !S_ISDIR(childNodes[i]->mode))
			{
				errno = ENOTDIR;
				return -1;
			}

			if (!childNodes[i]->onUnlink())
			{
				return -1;
			}

			childNodes[i] = vnode;
			if (oldDirectory == this)
			{
				unlinkUnlocked(oldName, 0);
			}
			else
			{
				oldDirectory->unlink(oldName, 0);
				if (S_ISDIR(vnode->mode))
				{
					((Reference<DirectoryVnode>) vnode)->parent = this;
				}
			}
			return 0;
		}
	}

	if (oldDirectory == this)
	{
		for (size_t i = 0; i < childCount; i++)
		{
			if (strcmp(oldName, fileNames[i]) == 0)
			{
				char* name = strdup(newName);
				if (!name)
					return -1;
				free(fileNames[i]);
				fileNames[i] = name;
				return 0;
			}
		}
	}
	if (!addChildNodeUnlocked(newName, vnode))
		return -1;
	oldDirectory->unlink(oldName, 0);
	if (S_ISDIR(vnode->mode))
	{
		((Reference<DirectoryVnode>) vnode)->parent = this;
	}
	return 0;
}

int DirectoryVnode::unlink(const char* name, int flags)
{
	AutoLock lock(&mutex);
	return unlinkUnlocked(name, flags);
}

int DirectoryVnode::unlinkUnlocked(const char* name, int flags)
{
	for (size_t i = 0; i < childCount; i++)
	{
		if (strcmp(name, fileNames[i]) == 0)
		{
			Reference<Vnode> vnode = childNodes[i];

			if (flags)
			{
				if (S_ISDIR(vnode->mode) && !(flags & AT_REMOVEDIR))
				{
					errno = EPERM;
					return -1;
				}
				if (!S_ISDIR(vnode->mode) && !(flags & AT_REMOVEFILE))
				{
					errno = ENOTDIR;
					return -1;
				}
				if (!vnode->onUnlink())
					return -1;
			}
			free(fileNames[i]);
			if (i != childCount - 1)
			{
				childNodes[i] = childNodes[childCount - 1];
				fileNames[i] = fileNames[childCount - 1];
			}
			childNodes[--childCount].~Reference();

			Reference<Vnode>* newChildNodes = (Reference<Vnode>*) realloc(childNodes, childCount * sizeof(Reference<Vnode>));
			char** newFileNames = (char**) realloc(fileNames, childCount * sizeof(const char*));
			if (newChildNodes)
			{
				childNodes = newChildNodes;
			}
			if (newFileNames)
			{
				fileNames = newFileNames;
			}
			return 0;
		}
	}
	errno = ENOENT;
	return -1;
}
