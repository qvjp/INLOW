#include <stdio.h>

int fputs_unlocked(const char* restrict s, FILE* restrict file) {
    while (*s) {
        fputc_unlocked(*s++, file);
    }
    return 1;
}