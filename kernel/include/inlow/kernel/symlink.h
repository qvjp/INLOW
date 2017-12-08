#ifndef KERNEL_SYMLINK_H
#define KERNEL_SYMLINK_H

#include <inlow/kernel/vnode.h>

class SymlinkVnode : public Vnode
{
	public:
		SymlinkVnode(const char* target, ino_t ino, dev_t dev);
		~SymlinkVnode();
		virtual char* getLinkTarget();
	private:
		const char* target;
};

#endif
