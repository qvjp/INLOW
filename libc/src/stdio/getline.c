#include <stdio.h>

ssize_t getline(char** restrict lineptr, size_t* restrict size, FILE* restrict file)
{
	return getdelim(lineptr, size, '\n', file);
}
