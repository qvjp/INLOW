#include <string.h>
#include <inlow/kernel/file.h>

FileVnode::FileVnode(const void* data, size_t size)
{
		this->data = new char[size];
		memcpy(this->data, data, size);
		fileSize = size;
}

bool FileVnode::isSeekable()
{
	return true;
}

ssize_t FileVnode::pread(void* buffer, size_t size, off_t offset)
{
	char* buf = (char*) buffer;

	for (size_t i = 0; i < size; i++)
	{
		if (offset + i >= fileSize)
				return i;
		buf[i] = data[offset + i];
	}
	return size;
}
