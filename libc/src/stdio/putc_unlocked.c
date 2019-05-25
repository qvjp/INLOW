#include <stdio.h>

int putc_unlocked(int c, FILE* file) {
    return fputc_unlocked(c, file);
}