#include "FILE.h"

void clearerr_unlocked(FILE* file)
{
	file->flags &= ~(FILE_FLAG_EOF | FILE_FLAG_ERROR);
}
