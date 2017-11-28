#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char* getcwd(char* buffer, size_t size)
{
	if (buffer && size == 0)
	{
		errno = EINVAL;
		return NULL;
	}

	char* result = canonicalize_file_name(".");
	if (!result)
		return NULL;
	if (!buffer)
		return result;
	if (strlcpy(buffer, result, size) >= size)
	{
		free(result);
		errno = ERANGE;
		return NULL;
	}
	free(result);
	return buffer;
}
