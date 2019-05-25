#ifndef STDIO_H_
#define STDIO_H_

#include <sys/cdefs.h>
#define __need___va_list
#include <stdarg.h>
#define __need_FILE
#define __need_size_t

#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C"
{
#endif

extern FILE* stderr;
#define stderr stderr

/* These are just declared to make libgcc compile and are not implemented. */
int fflush(FILE*);
int fprintf(FILE* __restrict, const char* __restrict, ...);

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
int vfprintf(FILE* __restrict, const char* __restrict, __gnuc_va_list);

#ifdef __cplusplus
}
#endif

#endif
