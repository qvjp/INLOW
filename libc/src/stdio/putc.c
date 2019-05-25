#include <stdio.h>

int putc(int c, FILE* file) {
    flockfile(file);
    int result = putc_unlocked(c, file);
    funlockfile(file);
    return result;
}