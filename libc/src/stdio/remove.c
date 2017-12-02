#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int remove(const char* path)
{
	return unlinkat(AT_FDCWD, path, AT_REMOVEDIR | AT_REMOVEFILE);
}
