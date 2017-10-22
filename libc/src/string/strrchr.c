#include <string.h>

char* strrchr(const char* s, int c)
{
	const char* result = NULL;

	do
	{
		if (*s == (char) c)
				result = s;
	} while (*s++);

	return (char*) result;
}
