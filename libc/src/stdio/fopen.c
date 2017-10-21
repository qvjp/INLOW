#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

FILE* fopen(const char* restrict path, const char* restrict mode)
{
	int flags;

	if (strcmp(mode, "r") == 0 || strcmp(mode, "rb") == 0)
			flags = O_RDONLY;
	else if (strcmp(mode, "w") == 0 || strcmp(mode, "wb") == 0)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (strcmp(mode, "a") == 0 || strcmp(mode, "ab") == 0)
			flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (strcmp(mode, "r+") == 0 || strcmp(mode, "rb+") == 0 || strcmp(mode, "r+b") == 0)
			flags = O_RDWR;
	else if (strcmp(mode, "w+") == 0 || strcmp(mode, "wb+") == 0 || strcmp(mode, "w+b") == 0)
			flags = O_RDWR | O_CREAT | O_TRUNC;
	else if (strcmp(mode, "a+") == 0 || strcmp(mode, "ab+") == 0 || strcmp(mode, "a+b") == 0)
			flags = O_RDWR | O_CREAT | O_APPEND;
	else
	{
			errno = EINVAL;
			return NULL;
	}
	int fd = open(path, flags, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	return fdopen(fd, mode);
}
