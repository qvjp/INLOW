#include <stdbool.h>
#include <unistd.h>
#include <sys/stat.h>

int access(const char* path, int mode)
{
	struct stat st;
	if (stat(path, &st) < 0)
	{
		return -1;
	}

	bool accessible = true;
	if (mode & R_OK)
	{
		accessible &= (st.st_mode & (S_IRUSR | S_IRGRP | S_IROTH));
	}
	if (mode & W_OK)
	{
		accessible &= (st.st_mode & (S_IWUSR | S_IWGRP | S_IWOTH));
	}
	if (mode & X_OK)
	{
		accessible &= (st.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH));
	}

	return accessible ? 0 : -1;
}
