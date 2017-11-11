#include <stdio.h>

size_t fwrite(const void* restrict ptr, size_t size, size_t count, FILE* restrict file)
{
	flockfile(file);

	size_t result = fwrite_unlocked(ptr, size, count, file);
	funlockfile(file);
	return result;
}
