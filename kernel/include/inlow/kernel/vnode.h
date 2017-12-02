#ifndef KERNEL_VNODE_H
#define KERNEL_VNODE_H

#include <sys/types.h>
#include <inlow/stat.h>
#include <inlow/kernel/refcount.h>

class Vnode : public ReferenceCounted
{
	public:
			virtual int ftruncate(off_t length);
			virtual Reference<Vnode> getChildNode(const char* path);
			virtual bool isSeekable();
			virtual int mkdir(const char* name, mode_t mode);
			virtual bool onUnlink();
			virtual ssize_t pread(void* buffer, size_t size, off_t offset);
			virtual ssize_t pwrite(const void* buffer, size_t size, off_t offset);
			virtual ssize_t read(void* buffer, size_t size);
			virtual ssize_t readdir(unsigned long offset, void* buffer, size_t size);
			virtual int stat(struct stat* result);
			virtual int tcgetattr(struct termios* result);
			virtual int tcsetattr(int flags, const struct termios* termio);
			virtual int unlink(const char* name, int flags);
			virtual ssize_t write(const void* buffer, size_t size);
			virtual ~Vnode(){}
	protected:
			Vnode(mode_t mode, dev_t dev, ino_t ino);
	public:
			dev_t dev;
			ino_t ino;
			mode_t mode;
};

Reference<Vnode> resolvePath(const Reference<Vnode>& vnode, const char* path);

#endif
