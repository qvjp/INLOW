#ifndef _SYS_MMAN_H
#define _SYS_MMAN_H

#define __need_mode_t
#define __need_off_t
#define __need_size_t
#include <sys/libc-types.h>
#include <inlow/mman.h>

#ifdef __cplusplus
extern "C" {
#endif

void* mmap(void*, size_t, int, int, int, off_t);
int munmap(void*, size_t);

#ifdef __cplusplus
}
#endif

#endif