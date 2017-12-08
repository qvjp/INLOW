#include <fcntl.h>
#include <unistd.h>

int link(const char* oldPath, const char* newPath)
{
	return linkat(AT_FDCWD, oldPath, AT_FDCWD, newPath, 0);
}
