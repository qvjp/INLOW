#include <stdio.h>

int fputc(int c, FILE* file) {
    flockfile(file);
    int result = fputc_unlocked(c, file);
    funlockfile(file);
    return result;
}