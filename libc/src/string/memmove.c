#include <string.h>
void* memmove(void* restrict dest, const void* restrict src, size_t size)
{
	unsigned char* d = dest;
	const unsigned char* s = src;

	if (src > dest)
	{
		for (size_t i = 0; i < size; i++)
		{
			d[i] = s[i];
		}
	}
	else
	{
		for (size_t i = size; i > 0; i--)
		{
			d[i-1] = s[i-1];
		}
	}
	return dest;
}
