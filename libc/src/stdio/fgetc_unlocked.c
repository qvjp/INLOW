#include <stdio.h>
#include <unistd.h>

int fgetc_unlocked(FILE* file)
{
	unsigned char result;
	read(file->fd, &result, 1);
	return result;
}
