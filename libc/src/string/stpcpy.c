#include <string.h>

char* stpcpy(char* restrict s1, const char* restrict s2)
{
	while (*s2)
			*s1++ = *s2++;
	*s1 = '\0';
	return s1;
}
