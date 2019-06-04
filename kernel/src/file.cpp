#include <string.h>
#include <inlow/kernel/file.h>

FileVnode::FileVnode() {
    data = "Hello World! I'm Foolish boy.";
    fileSize = strlen(data);
}

bool FileVnode::isSeekable() {
    return true;
}

ssize_t FileVnode::pread(void* buffer, size_t size, off_t offset) {
    char* buf = (char*) buffer;

    for (size_t i = 0; i < size; i++) {
        if (offset + i >= fileSize) return i;
        buf[i] = data[offset + i];
    }

    return size;
}
