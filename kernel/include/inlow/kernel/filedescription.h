#ifndef KERNEL_FILEDESCRIPTION_H
#define KERNEL_FILEDESCRIPTION_H

#include <inlow/kernel/vnode.h>

class FileDescription
{
	public:
			FileDescription(Vnode* vnode);
			ssize_t write(const void* buffer, size_t size);
	private:
			Vnode* vnode;
};

#endif
