#include <stdio.h>

size_t fwrite(const void* restrict ptr, size_t size, size_t count, FILE* restrict file)
{
	const unsigned char* p = (const unsigned char*) ptr;
	if (size == 0 || count == 0)
			return 0;
	flockfile(file);

	size_t i;
	for (i = 0; i < count; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (fputc_unlocked(p[i * size + j], file) < 0)
			{
				goto end;
			}
		}
	}
end:
	funlockfile(file);
	return i;
}
