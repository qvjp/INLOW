#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

static size_t integerToString(char* output, uintmax_t value, unsigned base);
static size_t noop(void*, const char*, size_t);

enum {
    LENGTH_CHAR,
    LENGTH_SHORT,
    LENGTH_INT,
    LENGTH_LONG,
    LENGTH_LONG_LONG,
    LENGTH_INTMAX,
    LENGTH_SIZE,
    LENGTH_PTRDIFF,
    LENGTH_LONG_DOUBLE
};

enum {
    SPECIFIER_SIGNED = 1,
    SPECIFIER_OCTAL,
    SPECIFIER_UNSIGNED,
    SPECIFIER_HEX,
    SPECIFIER_HEX_CAPITAL,
    SPECIFIER_CHAR,
    SPECIFIER_STRING,
    SPECIFIER_POINTER,
    SPECIFIER_N,
    SPECIFIER_PERCENT_SIGN
};

int vcbprintf(void* param, size_t (*callback)(void*, const char*, size_t),
        const char* format, va_list ap) {
    if (!callback) {
        callback = noop;
    }

    char buffer[sizeof(uintmax_t) * 3];
    int result = 0;

    while (*format) {
        if (*format != '%') {
            if (callback(param, format, 1) != 1) return -1;
            result += 1;
        } else {
            int length = LENGTH_INT;
            int specifier = 0;

            while (specifier == 0) {
                switch (*++format) {
                //TODO: Parse flags

                // Parse length modifiers
                case 'h':
                    length = length == LENGTH_SHORT ? LENGTH_CHAR : LENGTH_SHORT;
                    break;
                case 'l':
                    length = length == LENGTH_LONG ? LENGTH_LONG_LONG : LENGTH_LONG;
                    break;
                case 'j':
                    length = LENGTH_INTMAX;
                    break;
                case 'z':
                    length = LENGTH_SIZE;
                    break;
                case 't':
                    length = LENGTH_PTRDIFF;
                    break;
                case 'L':
                    length = LENGTH_LONG_DOUBLE;
                    break;

                // Parse specifiers
                case '%':
                    specifier = SPECIFIER_PERCENT_SIGN;
                    break;
                case 'd':
                case 'i':
                    specifier = SPECIFIER_SIGNED;
                    break;
                case 'o':
                    specifier = SPECIFIER_OCTAL;
                    break;
                case 'u':
                    specifier = SPECIFIER_UNSIGNED;
                    break;
                case 'x':
                    specifier = SPECIFIER_HEX;
                    break;
                case 'X':
                    specifier = SPECIFIER_HEX_CAPITAL;
                    break;
                case 'c':
                    specifier = SPECIFIER_CHAR;
                    break;
                case 's':
                    specifier = SPECIFIER_STRING;
                    break;
                case 'p':
                    specifier = SPECIFIER_POINTER;
                    break;
                case 'n':
                    specifier = SPECIFIER_N;
                    break;
                case '\0':
                    if (callback(param, "%", 1) != 1) return -1;
                    return result + 1;
                default:
                    break;
                }
            }

            uintmax_t value;
            if (specifier != SPECIFIER_CHAR
                    && specifier != SPECIFIER_STRING
                    && specifier != SPECIFIER_POINTER
                    && specifier != SPECIFIER_N
                    && specifier != SPECIFIER_PERCENT_SIGN) {
                switch (length) {
                case LENGTH_CHAR:
                    value = (unsigned char) va_arg(ap, unsigned int);
                    break;
                case LENGTH_SHORT:
                    value = (unsigned short) va_arg(ap, unsigned int);
                    break;
                case LENGTH_INT:
                    value = va_arg(ap, unsigned int);
                    break;
                case LENGTH_LONG:
                    value = va_arg(ap, unsigned long);
                    break;
                case LENGTH_LONG_LONG:
                    value = va_arg(ap, unsigned long long);
                    break;
                case LENGTH_INTMAX:
                    value = va_arg(ap, uintmax_t);
                    break;
                case LENGTH_SIZE:
                    value = va_arg(ap, size_t);
                    break;
                case LENGTH_PTRDIFF:
                    value = va_arg(ap, ptrdiff_t);
                    break;
                default:
                    value = 0;
                }
            }

            size_t size;
            char c;
            const char* s;

            switch (specifier) {
            case SPECIFIER_SIGNED:
                //TODO: Implement %d
                break;
            case SPECIFIER_OCTAL:
                size = integerToString(buffer, value, 8);
                if (callback(param, buffer, size) != size) return -1;
                result += size;
                break;
            case SPECIFIER_UNSIGNED:
                size = integerToString(buffer, value, 10);
                if (callback(param, buffer, size) != size) return -1;
                result += size;
                break;
            case SPECIFIER_HEX:
                size = integerToString(buffer, value, 16);
                if (callback(param, buffer, size) != size) return -1;
                result += size;
                break;
            case SPECIFIER_HEX_CAPITAL:
                //TODO: Implement %X
                break;
            case SPECIFIER_CHAR:
                c = (char) va_arg(ap, int);
                if (callback(param, &c, 1) != 1) return -1;
                result += 1;
                break;
            case SPECIFIER_STRING:
                s = va_arg(ap, const char*);
                size = strlen(s);
                if (callback(param, s, size) != size) return -1;
                result += size;
                break;
            case SPECIFIER_POINTER:
                value = (uintptr_t) va_arg(ap, void*);
                size_t size = integerToString(buffer, value, 16);
                if (callback(param, buffer, size) != size) return -1;
                result += size;
                break;
            case SPECIFIER_N:
                //TODO: Implement %n
                break;
            case SPECIFIER_PERCENT_SIGN:
                if (callback(param, "%", 1) != 1) return -1;
                result += 1;
                break;
            }
        }
        format++;
    }

    return result;
}

static size_t integerToString(char* output, uintmax_t value, unsigned base) {
    static const char* digits = "0123456789abcdef";

    // Calculate the length of the output string
    uintmax_t i = value;
    size_t length = 0;

    while (i > 0) {
        i /= base;
        length++;
    }

    if(length == 0) length = 1;

    output += length;
    *output = '\0';

    do {
        *--output = digits[value % base];
        value /= base;
    } while (value);

    return length;
}

static size_t noop(void* param, const char* s, size_t nBytes){
    (void) param; (void) s;
    return nBytes;
}