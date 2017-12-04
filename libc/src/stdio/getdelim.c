#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

ssize_t getdelim(char** restrict lineptr, size_t* restrict size, int delimiter, FILE* restrict file)
{
	if (!lineptr || !size)
	{
		errno = EINVAL;
		return -1;
	}

	if (!*lineptr || !*size)
	{
		*size = 80;
		char* newBuffer = realloc(*lineptr, *size);
		if (!newBuffer)
		{
			*size = 0;
			return -1;
		}
		*lineptr = newBuffer;
	}

	flockfile(file);

	size_t i = 0;
	int c;
	do
	{
		if (i + 1 >= *size)
		{
			char* newBuffer = reallocarray(*lineptr, 2, *size);
			if (!newBuffer)
			{
				funlockfile(file);
				return -1;
			}
			*lineptr = newBuffer;
			*size *= 2;
		}

		c = fgetc_unlocked(file);
		if (c == EOF)
		{
			(*lineptr)[i] = '\0';
			 funlockfile(file);
			 return i > 0 ? (ssize_t) i : -1;
		}

		(*lineptr)[i++] = c;
	} while (c != delimiter);

	(*lineptr)[i] = '\0';
	 funlockfile(file);
	 return i;
}
