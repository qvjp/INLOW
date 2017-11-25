#ifndef KERNEL_FILE_H
#define KERNEL_FILE_H

#include <inlow/kernel/kthread.h>
#include <inlow/kernel/vnode.h>

class FileVnode : public Vnode
{
	public:
			FileVnode(const void* data, size_t size, mode_t mode);
			~FileVnode();
			virtual int ftruncate(off_t length);
			virtual bool isSeekable();
			virtual ssize_t pread(void* buffer, size_t size, off_t offset);
			virtual ssize_t pwrite(const void* buffer, size_t size, off_t offset);
	public:
			char* data;
			size_t fileSize;
			kthread_mutex_t mutex;
};

#endif
