#include <stdio.h>

int getchar_unlocked(void) {
    return getc_unlocked(stdin);
}