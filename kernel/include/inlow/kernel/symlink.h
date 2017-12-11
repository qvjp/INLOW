#ifndef KERNEL_SYMLINK_H
#define KERNEL_SYMLINK_H

#include <inlow/kernel/vnode.h>

class SymlinkVnode : public Vnode
{
	public:
		SymlinkVnode(const char* target, dev_t dev, ino_t ino);
		SymlinkVnode(const char* target, size_t targetLength, dev_t dev, ino_t ino);
		~SymlinkVnode();
		virtual char* getLinkTarget();
	private:
		const char* target;
};

#endif
