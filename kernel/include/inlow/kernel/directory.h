#ifndef KERNEL_DIRECTORY_H
#define KERNEL_DIRECTORY_H

#include <inlow/kernel/vnode.h>

class DirectoryVnode : public Vnode
{
	public:
			DirectoryVnode(DirectoryVnode* parent, mode_t mode);
			~DirectoryVnode();
			bool addChildNode(const char* path, Vnode* vnode);
			virtual Vnode* openat(const char* path, int flags, mode_t mode);
			virtual ssize_t readdir(unsigned long offset, void* buffer, size_t size);
	public:
			size_t childCount;
	private:
			Vnode** childNodes;
			const char** fileNames;
			DirectoryVnode* parent;
};

#endif
