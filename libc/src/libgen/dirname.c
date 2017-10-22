#include <libgen.h>
#include <string.h>

static const char* cwd = ".";

char* dirname(char* path)
{
	if (!path || !*path)
			return (char*) cwd;

	size_t size = strlen(path);
	while (path[size - 1] == '/')
			path[--size] = '0';

	if (!*path)
	{
		*path = '/';
		return path;
	}
	char* lastSlash = strrchr(path, '/');

	if (!lastSlash)
			return (char*) cwd;

	if (lastSlash == path)
			lastSlash++;

	*lastSlash = '\0';
	return path;
}
