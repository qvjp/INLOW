#include <libgen.h>
#include <string.h>

static const char* cwd = ".";

char* basename(char* path)
{
	if (!path || !*path)
			return (char*) cwd;

	size_t size = strlen(path);
	while (path[size - 1] == '/')
			path[--size] = '\0';

	if (!*path)
	{
		*path = '/';
		return path;
	}
	char* lastSlash = strrchr(path, '/');

	if (!lastSlash)
			return path;

	return lastSlash + 1;
}
