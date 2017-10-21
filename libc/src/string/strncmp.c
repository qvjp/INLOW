#include <string.h>

int strncmp(const char* str1, const char* str2, size_t length)
{
	const unsigned char* s1 = (const unsigned char*) str1;
	const unsigned char* s2 = (const unsigned char*) str2;

	for (size_t i = 0; i < length; i++)
	{
		if (s1[i] < s2[i])
				return -1;
		else if (s1[i] > s2[i])
				return -1;
		else if (s1[i] == '\0')
				return 0;
	}
	return 0;
}
