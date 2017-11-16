#include <errno.h>
#include <stdlib.h>
#include <string.h>

void* calloc(size_t size1, size_t size2)
{
	size_t resultSize;
	if (__builtin_mul_overflow(size1, size2, &resultSize))
	{
		errno = ENOMEM;
		return NULL;
	}

	void* result = malloc(resultSize);
					
	if (result)
			memset(result, 0, resultSize);
	return result;
}
