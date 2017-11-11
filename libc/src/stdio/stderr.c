#include "FILE.h"

static FILE __stderr = 
{
	.fd = 2,
	.flags = 0,
};

FILE* stderr = &__stderr;
