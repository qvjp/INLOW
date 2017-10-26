#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/cdefs.h>
#define __need_NULL
#define __need_size_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C" {
#endif

		void* calloc(size_t, size_t);
		__noreturn void _Exit(int);
		__noreturn void exit(int);

		void free(void*);
		void* malloc(size_t);
		void* realloc(void*, size_t);
		long strtol(const char* __restrict, char** __restrict, int);
		unsigned long strtoul(const char* __restrict, char** __restrict, int);


		__noreturn void abort(void);

		int atexit(void(*)(void));
		int atoi(const char*);
		char* getenv(const char*);

#ifdef __cplusplus
}
#endif

#endif
