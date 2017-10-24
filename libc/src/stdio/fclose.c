#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int fclose(FILE* file)
{
	if (fflush(file) == EOF)
			return EOF;
	if (close(file->fd) == -1)
			return EOF;
	free(file);
	return 0;
}
