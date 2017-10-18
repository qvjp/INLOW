#include <stdio.h>

FILE __stdin = 
{
	.fd = 0,
};

FILE* stdin = &__stdin;
