#ifndef KERNEL_VNODE_H
#define KERNEL_VNODE_H

#include <sys/types.h>
#include <inlow/stat.h>

class Vnode
{
	public:
			virtual bool isSeekable();
			virtual Vnode* openat(const char* path, int flags, mode_t mode);
			virtual ssize_t pread(void* buffer, size_t size, off_t offset);
			virtual ssize_t read(void* buffer, size_t size);
			virtual ssize_t readdir(unsigned long offset, void* buffer, size_t size);
			virtual int stat(struct stat* result);
			virtual int tcgetattr(struct termios* result);
			virtual int tcsetattr(int flags, const struct termios* termio);
			virtual ssize_t write(const void* buffer, size_t size);
			virtual ~Vnode(){}
	protected:
			Vnode(mode_t mode);
	public:
			mode_t mode;
};

#endif
