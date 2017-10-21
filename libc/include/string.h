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
		int strcmp(const char*, const char*);
		size_t strcspn(const char*, const char*);
		size_t strlen(const char*);
		int strncmp(const char*, const char*, size_t);

		char* strcpy(char* __restrict, const char* __restrict);

#ifdef __cplusplus
}
#endif


#endif
