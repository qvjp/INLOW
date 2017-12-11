#include <string.h>

size_t strnlen(const char* s, size_t maxlen)
{
	size_t length = 0;
	while (length < maxlen && s[length] != '\0')
	{
		length++;
	}
	return length;
}
