#include <stdlib.h>
#include <string.h>

void* calloc(size_t size1, size_t size2)
{
	void* result = malloc(size1 * size2);
	if (result)
			memset(result, 0, size1 * size2);
	return result;
}
