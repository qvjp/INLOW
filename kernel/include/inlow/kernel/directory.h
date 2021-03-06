#ifndef KERNEL_DIRECTORY_H
#define KERNEL_DIRECTORY_H

#include <inlow/kernel/kthread.h>
#include <inlow/kernel/vnode.h>

class DirectoryVnode : public Vnode
{
	public:
			DirectoryVnode(const Reference<DirectoryVnode>& parent, mode_t mode, dev_t dev, ino_t ino);
			~DirectoryVnode();
			virtual Reference<Vnode> getChildNode(const char* name);
			virtual int link(const char* name, const Reference<Vnode>& vnode);
			virtual int mkdir(const char* name, mode_t mode);
			virtual bool onUnlink();
			virtual ssize_t readdir(unsigned long offset, void* buffer, size_t size);
			virtual int rename(Reference<Vnode>& oldDirectory, const char* oldName, const char* newName);
			virtual int unlink(const char* path, int flags);
	private:
			Reference<Vnode> getChildNodeUnlocked(const char* name);
			int linkUnlocked(const char* name, const Reference<Vnode>& vnode);
			int unlinkUnlocked(const char* path, int flags);
	public:
			size_t childCount;
	private:
			Reference<Vnode>* childNodes;
			char** fileNames;
			kthread_mutex_t mutex;
			Reference<DirectoryVnode> parent;
};

#endif
