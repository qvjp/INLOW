#include <fcntl.h>
#include <unistd.h>

int symlink(const char* targetPath, const char* linkPath)
{
	return symlinkat(targetPath, AT_FDCWD, linkPath);
}
