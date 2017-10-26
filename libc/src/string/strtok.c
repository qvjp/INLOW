#include <string.h>

static char* next = NULL;

char* strtok(char* restrict str, const char* restrict seperators)
{
	if (!str)
	{
		str = next;
		if (!str)
				return NULL;
	}
	size_t tokenEnd = strcspn(str, seperators);
	if (str[tokenEnd] == '\0')
	{
		next = NULL;
	}
	else
	{
		str[tokenEnd] = '\0';
		next = str + tokenEnd + 1;
	}
	return str;
}
