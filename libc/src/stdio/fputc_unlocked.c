#include <unistd.h>
#include "FILE.h"

int fputc_unlocked(int c, FILE* file)
{
	if (write(file->fd, &c, 1) < 0)
	{
		file->flags |= FILE_FLAG_ERROR;
		return EOF;
	}
	return c;
}
