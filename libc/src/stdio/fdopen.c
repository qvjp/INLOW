#include <stdio.h>
#include <stdlib.h>

FILE* fdopen(int fd, const char* mode)
{
	(void) mode;

	FILE* file = malloc(sizeof(FILE));
	file->fd = fd;
	return file;
}
