#include <stdarg.h>
#include <stdio.h>

int fprintf(FILE* restrict file, const char* restrict format, ...) {
    flockfile(file);
    va_list ap;
    va_start(ap, format);
    int result = vfprintf_unlocked(file, format, ap);
    va_end(ap);
    funlockfile(file);
    return result;
}