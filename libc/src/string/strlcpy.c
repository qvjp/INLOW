#include <string.h>

size_t strlcpy(char* restrict dest, const char* restrict source, size_t size)
{
	size_t i = 0;
	for (; i < size- 1; i++)
	{
		dest[i] = source[i];
		if (source[i] == '\0')
				return i;
	}
	dest[i] = '\0';
	while (source[i] != '\0')
			i++;
	return i;
}
