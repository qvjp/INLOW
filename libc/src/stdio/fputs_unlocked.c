#include <stdio.h>

int fputs_unlocked(const char* restrict s, FILE* restrict file)
{
	while (*s)
	{
		if (fputc_unlocked(*s++, file) < 0)
				return EOF;
	}
	return 1;
}
