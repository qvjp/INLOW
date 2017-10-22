#include <stdlib.h>
#include <string.h>

char* strdup(const char* str)
{
	size_t length = strlen(str);
	char* result = malloc(length + 1);
	memcpy(result, str, length + 1);
	return result;
}
