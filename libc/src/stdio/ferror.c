#include <stdio.h>

int ferror(FILE* file)
{
	flockfile(file);
	int result = ferror_unlocked(file);
	funlockfile(file);
	return result;
}
