#include <fcntl.h>
#include <sys/stat.h>

int stat(const char* restrict path, struct stat* restrict result)
{
	return fstatat(AT_FDCWD, path, result, 0);
}
