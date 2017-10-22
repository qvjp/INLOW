#ifndef KERNEL_FILE_H
#define KERNEL_FILE_H

#include <inlow/kernel/vnode.h>

class FileVnode : public Vnode
{
	public:
			FileVnode(const void* data, size_t size);
			virtual bool isSeekable();
			virtual ssize_t pread(void* buffer, size_t size, off_t offset);
	public:
			char* data;
			size_t fileSize;
};

#endif
