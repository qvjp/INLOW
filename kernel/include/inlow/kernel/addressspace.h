#ifndef KERNEL_ADDRESSSPACE_H__
#define KERNEL_ADDRESSSPACE_H__

#include <stdint.h> /* uintptr_t */

// 物理地址类型
typedef uintptr_t paddr_t;
// 虚拟地址类型
typedef uintptr_t vaddr_t;
typedef struct _AddressSpace AddressSpace;

typedef vaddr_t (*fptrMap)(paddr_t, int);
typedef vaddr_t (*fptrMapAt)(int, ...);
typedef void (*fptrUnmap)(vaddr_t);
typedef void (*fptrInitialize)();
typedef paddr_t (*fptrGetPhysicalAddress)(vaddr_t);

typedef struct _AddressSpace
{
    AddressSpace* this;
    fptrGetPhysicalAddress getPhysicalAddress;
    fptrMap map;
    fptrMapAt mapAt;
    fptrUnmap unmap;
    fptrInitialize initialize;
}AddressSpace;

vaddr_t AddressSpace_map(paddr_t physicalAddress, int flags);
vaddr_t AddressSpace_mapAt(int type, ...);
void AddressSpace_unmap(vaddr_t virtualAddress);
void AddressSpace_initialize();
paddr_t AddressSpace_getPhysicalAddress(vaddr_t virtualAddress);
// 内核地址空间的全局变量
extern AddressSpace* kernelSpace;
#endif /* KERNEL_ADDRESSSPACE_H__ */
