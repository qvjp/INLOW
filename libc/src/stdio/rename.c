#include <fcntl.h>
#include <stdio.h>

int rename(const char* oldPath, const char* newPath)
{
	return renameat(AT_FDCWD, oldPath, AT_FDCWD, newPath);
}
