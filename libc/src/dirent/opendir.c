#include <dirent.h>
#include <fcntl.h>
#include <stddef.h>

DIR* opendir(const char* path)
{
	int fd = open(path, O_DIRECTORY | O_SEARCH);
	if (fd < 0)
			return NULL;
	return fdopendir(fd);
}
