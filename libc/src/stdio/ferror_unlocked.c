#include "FILE.h"

int ferror_unlocked(FILE* file)
{
	return file->flags & FILE_FLAG_ERROR;
}
