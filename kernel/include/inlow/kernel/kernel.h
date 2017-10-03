#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include <stdint.h>

#define PACKED __attribute__((__packed__))
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect((x), 0)

typedef struct _incomplete_type symbol_t;

typedef uintptr_t paddr_t;
typedef uintptr_t vaddr_t;
#endif
