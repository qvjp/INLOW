#ifndef KERNEL_FILEDESCRIPTION_H
#define KERNEL_FILEDESCRIPTION_H

#include <inlow/kernel/vnode.h>

class FileDescription
{
	public:
			FileDescription(const Reference<Vnode>& vnode);
			FileDescription* openat(const char* path, int flags, mode_t mode);
			ssize_t read(void* buffer, size_t size);
			ssize_t readdir(unsigned long offset, void* buffer, size_t size);
			int tcgetattr(struct termios* result);
			int tcsetattr(int flags, const struct termios* termio);
			ssize_t write(const void* buffer, size_t size);
	public:
			Reference<Vnode> vnode;
	private:
			off_t offset;
};

#endif
