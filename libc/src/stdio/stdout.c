#include "FILE.h"

static FILE __stdout = {
	.fd = 1,
	.flags = 0,
};

FILE* stdout = &__stdout;
