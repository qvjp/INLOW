#ifndef FILE_H
#define FILE_H

#include <stdio.h>

struct __FILE
{
	int fd;
	int flags;
};

#define FILE_FLAG_EOF (1 << 0)
#define FILE_FLAG_ERROR (1 << 1)

#endif
