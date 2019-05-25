#include <stdio.h>

char* fgets(char* restrict buffer, int size, FILE* restrict file) {
    flockfile(file);
    char* result = fgets_unlocked(buffer, size, file);
    funlockfile(file);
    return result;
}