#include <stdarg.h>
#include <stdio.h>

int printf(const char* restrict format, ...) {
    flockfile(stdout);
    va_list ap;
    va_start(ap, format);
    int result = vfprintf_unlocked(stdout, format, ap);
    va_end(ap);
    funlockfile(stdout);
    return result;
}