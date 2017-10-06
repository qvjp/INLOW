#include <string.h>

void* memcpy(void* restrict dest, const void* restrict src, size_t size)
{
	unsigned char* d = dest;
	const unsigned char* s = src;

	for (size_t i = 0; i < size; i++)
	{
		d[i] = s[i];
	}
	return dest;
}
