#include <stdio.h>

void clearerr(FILE* file)
{
	flockfile(file);
	clearerr_unlocked(file);
	funlockfile(file);
}
