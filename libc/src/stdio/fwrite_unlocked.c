#include <stdio.h>

size_t fwrite_unlocked(const void* restrict ptr, size_t size, size_t count, FILE* restrict file)
{
	if (size == 0 || count == 0)
			return 0;
	const unsigned char* p = (const unsigned char*) ptr;
	size_t i;
	for (i = 0; i < count; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (fputc_unlocked(p[i * size + j], file) == EOF)
			{
				return i;
			}
		}
	}
	return i;
}
