#include <stdio.h>

int getc(FILE* file) {
    flockfile(file);
    int result = getc_unlocked(file);
    funlockfile(file);
    return result;
}