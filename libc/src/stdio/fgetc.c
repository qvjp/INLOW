#include <stdio.h>

int fgetc(FILE* file) {
    flockfile(file);
    int result = fgetc_unlocked(file);
    funlockfile(file);
    return result;
}