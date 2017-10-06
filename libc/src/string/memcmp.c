#include <string.h>

int memcmp(const void* p1, const void* p2, size_t size)
{
		const unsigned char* a = p1;
		const unsigned char* b = p2;

		for(size_t i = 0; i < size; i++)
		{
				if (a[i] < b[i])
				{
						return -1;
				}
				else if (a[i] > b[i])
				{
						return 1;
				}
		}
		return 0;
}
