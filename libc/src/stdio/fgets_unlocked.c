#include <stdio.h>

char* fgets_unlocked(char* restrict buffer, int size, FILE* restrict file)
{
	int i = 0;
	while (i < size - 1)
	{
		int c = fgetc_unlocked(file);
		if (c == EOF)
				break;
		buffer[i++] = c;
		if (c == '\n')
				break;
	}
	if (i == 0)
			return NULL;

	buffer[i] = '\0';
	return buffer;
}
