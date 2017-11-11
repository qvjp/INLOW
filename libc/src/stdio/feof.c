#include <stdio.h>

int feof(FILE* file)
{
	flockfile(file);
	int result = feof_unlocked(file);
	funlockfile(file);
	return result;
}
