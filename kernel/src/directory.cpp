#include <errno.h>
#include <string.h>
#include <inlow/kernel/directory.h>

DirectoryVnode::DirectoryVnode()
{
	childCount = 0;
	childNodes = nullptr;
	fileNames = nullptr;
}

void DirectoryVnode::addChildNode(const char* path, Vnode* vnode)
{
	Vnode** newChildNodes = new Vnode*[childCount + 1];
	const char** newFileNames = new const char*[childCount + 1];
	memcpy(newChildNodes, childNodes, sizeof(Vnode*) * childCount);
	memcpy(newFileNames, fileNames, sizeof(const char*) * childCount);

	childNodes = newChildNodes;
	fileNames = newFileNames;

	childNodes[childCount] = vnode;
	fileNames[childCount] = path;
	childCount++;
}

Vnode* DirectoryVnode::openat(const char* path, int flags, mode_t mode)
{
	while (*path == '/')
		path++;

	size_t length = strcspn(path, "/");

	if (length == 0)
			return this;

	for (size_t i = 0; i < childCount; i++)
	{
		if (strncmp(path, fileNames[i], length) == 0)
		{
			if (path[length] == '\0')
				return childNodes[i];
			else
				return childNodes[i]->openat(path + length, flags, mode);			
		}
	}

	errno = ENOENT;
	return nullptr;
}
