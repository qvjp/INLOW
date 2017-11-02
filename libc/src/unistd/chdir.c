#include <fcntl.h>
#include <unistd.h>

int chdir(const char* path)
{
	return fchdirat(AT_FDCWD, path);
}
