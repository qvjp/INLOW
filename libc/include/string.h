#ifndef _STRING_H
#define _STRING_H

#define __need_NULL
#define __need_size_t
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif

		int memcmp(const void*, const void*, size_t);
		void* memcpy(void* __restrict, const void* __restrict, size_t);
		void* memmove(void* __restrict, const void* __restrict, size_t);
		void* memset(void*, int, size_t);

#ifdef __cplusplus
}
#endif


#endif
