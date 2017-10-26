#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/cdefs.h>
#define __need_NULL
#define __need_size_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C" {
#endif

#define EXIT_FAILURE 1
#define EXIT_SUCCESS 0

		void* calloc(size_t, size_t);
		__noreturn void _Exit(int);
		__noreturn void exit(int);

		void free(void*);
		char* getenv(const char*);
		void* malloc(size_t);
		void* realloc(void*, size_t);
		long strtol(const char* __restrict, char** __restrict, int);
		unsigned long strtoul(const char* __restrict, char** __restrict, int);


		__noreturn void abort(void);

		int atexit(void(*)(void));
		int atoi(const char*);

#ifdef __cplusplus
}
#endif

#endif
