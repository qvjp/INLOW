#include <dirent.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <sys/types.h>

DEFINE_SYSCALL(SYSCALL_READDIR, ssize_t, sys_readdir, (int, unsigned long, void*, size_t));

struct dirent* readdir(DIR* dir)
{
	if (!dir->dirent)
	{
		dir->dirent = malloc(sizeof(struct dirent));
		dir->dirent->d_reclen = sizeof(struct dirent);
	}

	size_t entrySize = dir->dirent->d_reclen;
	ssize_t size = sys_readdir(dir->fd, dir->offset, dir->dirent, entrySize);

	if (size < 0)
			return NULL;
	else if (size == 0)
			return NULL;
	else if ((size_t) size <= entrySize)
	{
		dir->offset++;
		return dir->dirent;
	}
	else
	{
		free(dir->dirent);
		dir->dirent = malloc((size_t) size);
		sys_readdir(dir->fd, dir->offset, dir->dirent, (size_t) size);
		dir->offset++;
		return dir->dirent;
	}
}
