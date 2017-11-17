#ifndef KERNEL_ADDRESSSPACE_H
#define KERNEL_ADDRESSSPACE_H

#include <inlow/mman.h>
#include <inlow/kernel/memorysegment.h>

class AddressSpace
{
	public:
			AddressSpace();
			~AddressSpace();
			void activate();
			AddressSpace* fork();
			paddr_t getPhysicalAddress(vaddr_t virtualAddress);
			vaddr_t mapFromOtherAddressSpace(AddressSpace* sourceSpace, vaddr_t sourceVirtualAddress, size_t size, int protection);
			vaddr_t mapMemory(size_t size, int protection);
			vaddr_t mapMemory(vaddr_t virtualAddress, size_t size, int protection);
			vaddr_t mapPhysical(paddr_t physicalAddress, size_t size, int protection);
			vaddr_t mapPhysical(vaddr_t virtualAddress, paddr_t physicalAddress, size_t size, int protection);
			void unmapMemory(vaddr_t virtualAddress, size_t size);
			void unmapPhysical(vaddr_t firstVirtualAddress, size_t size);
	private:
			vaddr_t map(paddr_t physicalAddress, int protection);
			vaddr_t mapAt(vaddr_t virtualAddress, paddr_t physicalAddress, int protection);
			vaddr_t mapAt(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int protection);
			vaddr_t mapAtWithFlags(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int flags);
			void unmap(vaddr_t virtualAddress);

	private:
			paddr_t pageDir;
			vaddr_t pageDirMapped;
			MemorySegment* firstSegment;
			AddressSpace* prev;
			AddressSpace* next;

	public:
			static void initialize();

	private:
			static AddressSpace _kernelSpace;
};

extern AddressSpace* kernelSpace;

#endif
