#include <string.h>
 
size_t strlen(const char* s)
{
	size_t result = 0;
	while (*s++)
			result++;
	return result;
}
