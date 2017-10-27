#ifndef _STRING_H
#define _STRING_H

#include <sys/cdefs.h>
#define __need_NULL
#define __need_size_t
#include <sys/libc-types.h>

#ifdef __cplusplus
extern "C"
{
#endif

		int memcmp(const void*, const void*, size_t);
		void* memcpy(void* __restrict, const void* __restrict, size_t);
		void* memmove(void*, const void*, size_t);
		void* memset(void*, int, size_t);
		char* strchr(const char*, int);
		int strcmp(const char*, const char*);
		char* strcpy(char* __restrict, const char* __restrict);
		size_t strcspn(const char*, const char*);
		size_t strlen(const char*);
		int strncmp(const char*, const char*, size_t);
		char* strrchr(const char*, int);
		char* strtok(char* __restrict, const char* __restrict);

#if __USE_INLOW || __USE_POSIX
		char* stpcpy(char* __restrict, const char* __restrict);
		char* strdup(const char*);
#endif

#ifdef __cplusplus
}
#endif


#endif
