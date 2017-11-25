#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <inlow/stat.h>
#include <inlow/kernel/file.h>

FileVnode::FileVnode(const void* data, size_t size, mode_t mode, dev_t dev, ino_t ino) : Vnode(S_IFREG | mode, dev, ino)
{
		this->data = (char*) malloc(size);
		memcpy(this->data, data, size);
		fileSize = size;
		mutex = KTHREAD_MUTEX_INITIALIZER;
}

FileVnode::~FileVnode()
{
	free(data);
}

int FileVnode::ftruncate(off_t length)
{
	if (length < 0 || length > __SIZE_MAX__)
	{
		errno = EINVAL;
		return -1;
	}

	AutoLock lock(&mutex);
	void* newData = realloc(data, (size_t) length);
	if (!newData)
	{
		errno = ENOSPC;
		return -1;
	}
	data = (char*) newData;

	if (length > fileSize)
		memset(data + fileSize, '\0', length - fileSize);

	fileSize = (size_t) length;
	return 0;
}

bool FileVnode::isSeekable()
{
	return true;
}

ssize_t FileVnode::pread(void* buffer, size_t size, off_t offset)
{
	AutoLock lock(&mutex);
	char* buf = (char*) buffer;

	for (size_t i = 0; i < size; i++)
	{
		if (offset + i >= fileSize)
				return i;
		buf[i] = data[offset + i];
	}
	return size;
}
ssize_t FileVnode::pwrite(const void* buffer, size_t size, off_t offset) 
{
    if (offset < 0) 
	{
        errno = EINVAL;
        return -1;
    }

    AutoLock lock(&mutex);
    size_t newSize;
    if (__builtin_add_overflow(offset, size, &newSize)) 
	{
        errno = ENOSPC;
        return -1;
    }

    if (newSize > fileSize) 
	{
        void* newData = realloc(data, newSize);
        if (!newData) 
		{
            errno = ENOSPC;
            return -1;
        }
        data = (char*) newData;
        fileSize = newSize;
    }

    memcpy(data + offset, buffer, size);
    return size;
}
