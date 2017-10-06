#ifndef KERNEL_ADDRESSSPACE_H
#define KERNEL_ADDRESSSPACE_H

#include <stddef.h>
#include <inlow/kernel/kernel.h>

#define PAGE_PRESENT (1 << 0)
#define PAGE_WRITABLE (1 << 1)
#define PAGE_USER (1 << 2)

class AddressSpace
{
	public:
			void activate();
			vaddr_t allocate(size_t nPages);
			AddressSpace* fork();
			void free (vaddr_t virtualAddress, size_t nPages);
			paddr_t getPhysicalAddress(vaddr_t virtualAddress);
			bool isFree(vaddr_t virtualAddress);
			bool isFree(size_t pdIndex, size_t ptIndex);
			vaddr_t map(paddr_t physicalAddress, int flags);
			vaddr_t mapAt(vaddr_t virtualAddress, paddr_t physicalAddress, int flags);
			vaddr_t mapAt(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int flags);
			vaddr_t mapRange(paddr_t* physicalAddresses, int flags);
			vaddr_t mapRangeAt(vaddr_t virtualAddress, paddr_t* physicalAddresses, int flags);
			void unmap(vaddr_t virtualAddress);

	private:
			paddr_t pageDir;
			AddressSpace* next;
	private:
			AddressSpace();
			static AddressSpace _kernelSpace;

	public:
			static void initialize();
};

extern AddressSpace* kernelSpace;

#endif
