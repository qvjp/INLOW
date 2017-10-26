#ifndef KERNEL_KERNEL_H
#define KERNEL_KERNEL_H

#include <stddef.h>
#include <stdint.h>

#define ALIGNED(alignment) __attribute__((__aligned__(alignment)))
#define NORETURN __attribute__((__noreturn__))
#define PACKED __attribute__((__packed__))
#define restrict __restrict
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define ALIGNUP(val, alignment) ((((val) - 1) &~((alignment) - 1)) + (alignment))
typedef struct _incomplete_type symbol_t;

typedef uintptr_t paddr_t;
typedef uintptr_t vaddr_t;
#endif
