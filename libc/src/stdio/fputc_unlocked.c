#include <stdio.h>
#include <unistd.h>

int fputc_unlocked(int c, FILE* file)
{
	if (write(file->fd, &c, 1) < 0)
			return EOF;
	return c;
}
