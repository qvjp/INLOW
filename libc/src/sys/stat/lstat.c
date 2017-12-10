#include <fcntl.h>
#include <sys/stat.h>

int lstat(const char* restrict path, struct stat* restrict result)
{
	return fstatat(AT_FDCWD, path, result, AT_SYMLINK_NOFOLLOW);
}
