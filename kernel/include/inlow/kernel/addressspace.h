#ifndef KERNEL_ADDRESSSPACE_H
#define KERNEL_ADDRESSSPACE_H

#include <stddef.h>
#include <stdint.h>

typedef uintptr_t paddr_t;
typedef uintptr_t vaddr_t;

class AddressSpace
{
	public:
			vaddr_t map(paddr_t physicalAddress, int flags);
			vaddr_t mapAt(vaddr_t virtualAddress, paddr_t physicalAddress, int flags);
			vaddr_t mapAt(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int flags);
			void unmap(vaddr_t virtualAddress);

	private:
			AddressSpace();
			static AddressSpace _kernelSpace;

	public:
			static void initialize();
};

extern AddressSpace* kernelSpace;

#endif
