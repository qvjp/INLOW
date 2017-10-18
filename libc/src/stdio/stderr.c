#include <stdio.h>

FILE __stderr = 
{
	.fd = 2,
};

FILE* stderr = &__stderr;
