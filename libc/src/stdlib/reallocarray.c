#include <errno.h>
#include <stdlib.h>

void* reallocarray(void* ptr, size_t size1, size_t size2)
{
	size_t resultSize;
	if(__builtin_mul_overflow(size1, size2, &resultSize))
	{
		errno = ENOMEM;
		return NULL;
	}
	return realloc(ptr, resultSize);
}

