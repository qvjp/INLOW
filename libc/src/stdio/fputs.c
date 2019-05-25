#include <stdio.h>

int fputs(const char* restrict s, FILE* restrict file) {
    flockfile(file);
    int result = fputs_unlocked(s, file);
    funlockfile(file);
    return result;
}