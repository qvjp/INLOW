#ifndef _STDLIB_H
#define _STDLIB_H

#include <sys/cdefs.h>
#define __need_NULL
#define __need_size_t
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

		__noreturn void _Exit(int);
		__noreturn void exit(int);
		void free(void*);
		void* malloc(size_t);

#ifdef __cplusplus
}
#endif

#endif
