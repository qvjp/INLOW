#include <string.h>

char* strcpy(char* restrict s1, const char* restrict s2)
{
	char* result = s1;
	while (*s2)
			*s1++ = *s2++;

	*s1 = '\0';
	return result;
}
