#ifndef KERNEL_PHYSICALMEMORY_H__
#define KERNEL_PHYSICALMEMORY_H__

#include <inlow/kernel/multiboot.h>
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect((x), 0)

// 物理地址类型
typedef uintptr_t paddr_t;
// 虚拟地址类型
typedef uintptr_t vaddr_t;

void Physicalmemory_initialize(struct multiboot_info* multiboot);
void Physicalmemory_pushPageFrame(paddr_t physicalAddress);
paddr_t Physicalmemory_popPageFrame();


#endif /* KERNEL_PHYSICALMEMORY_H__ */