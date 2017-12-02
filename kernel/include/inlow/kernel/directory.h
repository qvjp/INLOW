#ifndef KERNEL_DIRECTORY_H
#define KERNEL_DIRECTORY_H

#include <inlow/kernel/kthread.h>
#include <inlow/kernel/vnode.h>

class DirectoryVnode : public Vnode
{
	public:
			DirectoryVnode(const Reference<DirectoryVnode>& parent, mode_t mode, dev_t dev, ino_t ino);
			~DirectoryVnode();
			bool addChildNode(const char* name, const Reference<Vnode>& vnode);
			virtual Reference<Vnode> getChildNode(const char* name);
			virtual int mkdir(const char* name, mode_t mode);
			virtual ssize_t readdir(unsigned long offset, void* buffer, size_t size);
	private:
			bool addChildNodeUnlocked(const char* name, const Reference<Vnode>& vnode);
			Reference<Vnode> getChildNodeUnlocked(const char* name);
	public:
			size_t childCount;
	private:
			Reference<Vnode>* childNodes;
			const char** fileNames;
			kthread_mutex_t mutex;
			Reference<DirectoryVnode> parent;
};

#endif
