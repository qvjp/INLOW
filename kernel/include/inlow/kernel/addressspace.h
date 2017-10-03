#ifndef KERNEL_ADDRESSSPACE_H
#define KERNEL_ADDRESSSPACE_H

#include <stddef.h>
#include <inlow/kernel/kernel.h>

#define PAGE_PRESENT 0x1
#define PAGE_WRITABLE 0x2

class AddressSpace
{
	public:
			paddr_t getPhysicalAddress(vaddr_t virtualAddress);
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
