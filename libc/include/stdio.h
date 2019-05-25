#ifndef STDIO_H_
#define STDIO_H_
#include <sys/cdefs.h>
#define __need___va_list
#include <stdarg.h>
#define __need_FILE
#define __need_fpos_t
#define __need_off_t
#define __need_size_t

#if __USE_INLOW || __USE_POSIX
#define __need_ssize_t
#endif

#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __is_inlow_libc
struct __FILE {
    int fd;
};
#endif

extern FILE* stdout;
#define stdout stdout

int fprintf(FILE* __restrict, const char* __restrict, ...);
int fputc(int, FILE*);
int fputs(const char* __restrict, FILE* __restrict);
int printf(const char* __restrict, ...);
int putc(int, FILE*);
int putchar(int);
int puts(const char*);
int vfprintf(FILE* __restrict, const char* __restrict, __gnuc_va_list);

#if __USE_INLOW || __USE_POSIX
void flockfile(FILE*);
void funlockfile(FILE*);
int putc_unlocked(int, FILE*);
int putchar_unlocked(int);
#endif

#if __USE_INLOW
int fputc_unlocked(int, FILE*);
int fputs_unlocked(const char* __restrict, FILE* __restrict);
int vcbprintf(void*, size_t (*)(void*, const char*, size_t), const char*,
        __gnuc_va_list);
int vfprintf_unlocked(FILE* __restrict, const char* __restrict,
        __gnuc_va_list);
#endif

/* These are just declared to make libgcc compile and are not implemented. */
extern FILE* stderr;
#define stderr stderr

int fflush(FILE*);

/* These are just declared to make libgcov compile, which is compiled with
   libgcc, and are not implemented. */
#define SEEK_SET 1
int fclose(FILE*);
FILE* fopen(const char* __restrict, const char* __restrict);
size_t fread(void* __restrict, size_t, size_t, FILE* __restrict);
int fseek(FILE*, long, int);
long ftell(FILE*);
size_t fwrite(const void* __restrict, size_t, size_t, FILE* __restrict);
void setbuf(FILE* __restrict, char* __restrict);

#ifdef __cplusplus
}
#endif

#endif