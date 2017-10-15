#include <stdio.h>
#include <unistd.h>

int fputc_unlocked(int c, FILE* file)
{
	write(file->fd, &c, 1);
	return c;
}
