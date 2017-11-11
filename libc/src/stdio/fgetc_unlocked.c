#include <unistd.h>
#include "FILE.h"

int fgetc_unlocked(FILE* file)
{
	if (file->flags & FILE_FLAG_EOF)
			return EOF;
	unsigned char result;
	ssize_t bytesRead = read(file->fd, &result, 1);
	if (bytesRead == 0)
	{
		file->flags |= FILE_FLAG_EOF;
		return EOF;
	}
	else if (bytesRead < 0)
	{
		file->flags |= FILE_FLAG_ERROR;
		return EOF;
	}
	return result;
}
