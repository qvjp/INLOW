#include <stdio.h>

FILE __stdout = {
    .fd = 1,
};

FILE* stdout = &__stdout;