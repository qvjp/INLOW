#ifndef _STDLIB_H
#define _STDLIB_H

#define __need_NULL
#define __need_size_t
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

void free(void*);
void* malloc(size_t);

#ifdef __cplusplus
}
#endif

#endif
