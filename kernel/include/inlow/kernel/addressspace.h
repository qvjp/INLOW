#ifndef KERNEL_ADDRESSSPACE_H
#define KERNEL_ADDRESSSPACE_H

#include <inlow/mman.h>
#include <inlow/kernel/kthread.h>
#include <inlow/kernel/memorysegment.h>

class AddressSpace
{
	public:
			AddressSpace();
			~AddressSpace();
			void activate();
			AddressSpace* fork();
			paddr_t getPhysicalAddress(vaddr_t virtualAddress);
			vaddr_t mapAt(vaddr_t virtualAddress, paddr_t physicalAddress, int protection);
			vaddr_t mapFromOtherAddressSpace(AddressSpace* sourceSpace, vaddr_t sourceVirtualAddress, size_t size, int protection);
			vaddr_t mapMemory(size_t size, int protection);
			vaddr_t mapMemory(vaddr_t virtualAddress, size_t size, int protection);
			vaddr_t mapPhysical(paddr_t physicalAddress, size_t size, int protection);
			void unmap(vaddr_t virtualAddress);
			void unmapMemory(vaddr_t virtualAddress, size_t size);
			void unmapPhysical(vaddr_t firstVirtualAddress, size_t size);
	private:
			vaddr_t mapAt(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int protection);
			vaddr_t mapAtWithFlags(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int flags);

	public:
			MemorySegment* firstSegment;

	private:
			paddr_t pageDir;
			vaddr_t pageDirMapped;
			AddressSpace* prev;
			AddressSpace* next;
			kthread_mutex_t mutex;

	public:
			static void initialize();

	private:
			static AddressSpace _kernelSpace;
};

extern AddressSpace* kernelSpace;

#endif
