#include <stdlib.h>
#include <string.h>

char* strndup(const char* str, size_t size)
{
	size_t length = strnlen(str,size);
	char* result = malloc(length + 1);
	memcpy(result, str, length);
	result[length] = '\0';
	return result;
}
