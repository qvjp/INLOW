#include <stdlib.h>
#include <string.h>
#include <inlow/kernel/symlink.h>

SymlinkVnode::SymlinkVnode(const char* target, ino_t ino, dev_t dev) : Vnode(S_IFLNK | 0777, dev, ino)
{
	this->target = strdup(target);
}

SymlinkVnode::~SymlinkVnode()
{
	free((char*) target);
}

char* SymlinkVnode::getLinkTarget()
{
	return strdup(target);
}
