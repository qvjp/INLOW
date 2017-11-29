#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <inlow/dirent.h>
#include <inlow/stat.h>
#include <inlow/kernel/directory.h>
#include <inlow/kernel/print.h>

DirectoryVnode::DirectoryVnode(DirectoryVnode* parent, mode_t mode, dev_t dev, ino_t ino) : Vnode(S_IFDIR | mode, dev, ino)
{
	childCount = 0;
	childNodes = nullptr;
	fileNames = nullptr;
	mutex = KTHREAD_MUTEX_INITIALIZER;
	this->parent = parent;
}

DirectoryVnode::~DirectoryVnode()
{
	free(childNodes);
	free(fileNames);
}

bool DirectoryVnode::addChildNode(const char* name, Vnode* vnode)
{
	AutoLock lock(&mutex);
	return addChildNodeUnlocked(name, vnode);
}

bool DirectoryVnode::addChildNodeUnlocked(const char* name, Vnode* vnode)
{
	Vnode** newChildNodes = (Vnode**) reallocarray(childNodes, childCount + 1, sizeof(Vnode*));
	if (!newChildNodes)
			return false;
	const char** newFileNames = (const char**) reallocarray(fileNames, childCount + 1, sizeof(const char*));
	if (!newFileNames)
			return false;
	childNodes = newChildNodes;
	fileNames = newFileNames;

	childNodes[childCount] = vnode;
	fileNames[childCount] = strdup(name);
	childCount++;
	return true;
}

Vnode* DirectoryVnode::getChildNode(const char* name)
{
	AutoLock lock(&mutex);
	return getChildNodeUnlocked(name);
}

Vnode* DirectoryVnode::getChildNodeUnlocked(const char* name)
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

	Vnode* newDirectory = new DirectoryVnode(this, mode, dev, 0);
	if (!addChildNodeUnlocked(name, newDirectory))
	{
		delete newDirectory;
		return -1;
	}
	return 0;
}

ssize_t DirectoryVnode::readdir(unsigned long offset, void* buffer, size_t size)
{
	AutoLock lock(&mutex);
	const char* name;
	Vnode* vnode;
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
