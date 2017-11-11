#include "FILE.h"

static FILE __stdin = 
{
	.fd = 0,
	.flags = 0,
};

FILE* stdin = &__stdin;
