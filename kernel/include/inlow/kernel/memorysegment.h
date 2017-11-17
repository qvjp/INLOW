#ifndef KERNEL_MEMORYSEGMENT_H
#define KERNEL_MEMORYSEGMENT_H

#include <inlow/kernel/kernel.h>

#define SEG_NOUNMAP (1 << 16)

class MemorySegment
{
	public:
			MemorySegment(vaddr_t address, size_t size, int flags, MemorySegment* prev, MemorySegment* next);

	public:
			vaddr_t address;
			size_t size;
			int flags;
			MemorySegment* prev;
			MemorySegment* next;

	public:
			static void addSegment(MemorySegment* firstSegment, vaddr_t address, size_t size, int protection);
			static void removeSegment(MemorySegment* firstSegment, vaddr_t address, size_t size);
			static vaddr_t findFreeSegment(MemorySegment* firstSegment, size_t size);
			static vaddr_t findAndAddNewSegment(MemorySegment* firstSegment, size_t size, int protection);

	private:
			static void addSegment(MemorySegment* firstSegment, MemorySegment* newSegment);
			static MemorySegment* allocateSegment(vaddr_t address, size_t size, int flags);
			static void deallocateSegment(MemorySegment* segment);
			static void verifySegmentList();
};

#endif
