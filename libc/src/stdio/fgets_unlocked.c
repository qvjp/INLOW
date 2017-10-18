#include <stdio.h>

char* fgets_unlocked(char* restrict buffer, int size, FILE* restrict file)
{
	int i = 0;
	for ( ; i < size - 1; i++)
	{
		int c = fgetc_unlocked(file);
		if (c == '\n')
				break;
		buffer[i] = c;
	}
	buffer[i] = '\0';
	return buffer;
}
