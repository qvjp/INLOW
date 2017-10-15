#include <inlow/kernel/filedescription.h>

FileDescription::FileDescription(Vnode* vnode)
{
	this->vnode = vnode;
}

ssize_t FileDescription::read(void* buffer, size_t size)
{
	return vnode->read(buffer, size);
}
ssize_t FileDescription::write(const void* buffer, size_t size)
{
	return vnode->write(buffer, size);
}
