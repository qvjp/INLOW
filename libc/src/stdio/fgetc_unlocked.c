#include <stdio.h>
#include <unistd.h>

int fgetc_unlocked(FILE* file) {
    unsigned char result;
    if (read(file->fd, &result, 1) < 1) {
        return EOF;
    }
    return result;
}