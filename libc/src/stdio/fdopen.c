#include <stdlib.h>
#include "FILE.h"

FILE* fdopen(int fd, const char* mode)
{
	(void) mode;

	FILE* file = malloc(sizeof(FILE));
	file->fd = fd;
	file->flags = 0;
	return file;
}
