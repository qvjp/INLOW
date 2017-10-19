#ifndef __INLOW_MMAN_H
#define __INLOW_MMAN_H

#define PROT_READ (1 << 0)
#define PROT_WRITE (1 << 1)
#define PROT_EXEC (1 << 2)
#define PROT_NONE 0

#define MAP_PRIVATE (1 << 0)
#define MAP_ANONYMOUS (1 << 1)

#define MAP_FAILED ((void*) 0)

#if defined(__is_inlow_kernel) || defined(__is_inlow_libc)
#include <stddef.h>
#include <inlow/types.h>

struct __mmapRequest
{
	void* _addr;
	size_t _size;
	int _protection;
	int _flags;
	int _fd;
	__off_t _offset;
};

#endif

#endif
