#include "FILE.h"

int feof_unlocked(FILE* file)
{
	return file->flags & FILE_FLAG_EOF;
}
