#include <dirent.h>
#include <stdlib.h>

DIR* fdopendir(int fd)
{
	DIR* dir = malloc(sizeof(DIR));
	dir->fd = fd;
	dir->offset = 0;
	dir->dirent = NULL;
	return dir;
}
