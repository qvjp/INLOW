#include <stdlib.h>
#include <string.h>
#include <inlow/kernel/symlink.h>

SymlinkVnode::SymlinkVnode(const char* target, dev_t dev, ino_t ino) : Vnode(S_IFLNK | 0777, dev, ino)
{
	this->target = strdup(target);
}

SymlinkVnode::SymlinkVnode(const char* target, size_t targetLength, dev_t dev, ino_t ino) : Vnode(S_IFLNK | 0777, dev, ino)
{
	this->target = strndup(target, targetLength);
}

SymlinkVnode::~SymlinkVnode()
{
	free((char*) target);
}

char* SymlinkVnode::getLinkTarget()
{
	return strdup(target);
}
