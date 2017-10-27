#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

int closedir(DIR* dir)
{
	if (close(dir->fd) == -1)
			return -1;
	if (dir->dirent)
			free(dir->dirent);
	free(dir);
	return 0;
}
