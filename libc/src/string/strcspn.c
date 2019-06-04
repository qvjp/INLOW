#include <string.h>

size_t strcspn(const char* string, const char* characters) {
    size_t result = 0;

    while (string[result]) {
        for (size_t i = 0; characters[i]; i++) {
            if (string[result] == characters[i]) {
                return result;
            }
        }

        result++;
    }

    return result;
}