#include <stdio.h>

int getc_unlocked(FILE* file) {
    return fgetc_unlocked(file);
}