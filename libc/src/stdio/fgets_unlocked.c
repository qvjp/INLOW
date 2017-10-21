#include <stdio.h>

char* fgets_unlocked(char* restrict buffer, int size, FILE* restrict file)
{
	int i = 0;
	for ( ; i < size - 1; i++)
	{
		int c = fgetc_unlocked(file);
		if (c == '\n' || c == EOF)
				break;
		buffer[i] = c;
	}
	if (i == 0)
			return NULL;

	buffer[i] = '\0';
	return buffer;
}
