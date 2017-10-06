#include <string.h>

void* memset(void* dest, int value, size_t size)
{
	unsigned char* p = dest;

	for (size_t i = 0; i < size; i++)
	{
		p[i] = (unsigned char) value;
	}
	return dest;
}
