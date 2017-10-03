#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/kernel.h>
#include <inlow/kernel/print.h>

#define RECURSIVE_MAPPING 0xFFC00000

extern "C"
{
	extern symbol_t bootstrapBegin;
	extern symbol_t bootstrapEnd;
}

AddressSpace AddressSpace::_kernelSpace;
AddressSpace* kernelSpace;

static inline vaddr_t indexToAddress(size_t pdIndex, size_t ptIndex)
{
	return (pdIndex << 22) | (ptIndex << 12);
}

static inline void addressToIndex(vaddr_t virtualAddress, size_t& pdIndex, size_t& ptIndex)
{
	pdIndex = virtualAddress >> 22;
	ptIndex = (virtualAddress >> 12) & 0x3FF;
}

AddressSpace::AddressSpace()
{

}

void AddressSpace::initialize()
{
	kernelSpace = &_kernelSpace;

	vaddr_t p = (vaddr_t) &bootstrapBegin;

	while (p < (vaddr_t) &bootstrapEnd)
	{
		kernelSpace->unmap(p);
		p += 0x1000;
	}
}

vaddr_t AddressSpace::map(paddr_t physicalAddress, int flags)
{
	// Find free page from the higher half amd map it
	for (size_t pdIndex = 0x300; pdIndex < 0x400; pdIndex++)
	{
		for (size_t ptIndex = 0; ptIndex < 0x400; ptIndex++)
		{
			uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);
			if (!*pageEntry)
			{
				return mapAt(pdIndex, ptIndex, physicalAddress, flags);
			}
		}
	}
	return 0;
}
vaddr_t AddressSpace::mapAt(vaddr_t virtualAddress, paddr_t physicalAddress, int flags)
{
	size_t pdIndex; 
	size_t ptIndex;
	addressToIndex(virtualAddress, pdIndex, ptIndex);
	return mapAt(pdIndex, ptIndex, physicalAddress, flags);
}
vaddr_t AddressSpace::mapAt(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int flags)
{
	if(this == kernelSpace)
	{
		uintptr_t* pageTable = (uintptr_t*)(RECURSIVE_MAPPING + 0x3FF000 + 4 * pdIndex);
		if (*pageTable)
		{
			uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);
			*pageEntry = physicalAddress | flags;
		}
		else
		{
			//TODO: Allocate a new page table;
				Print::printf("ERROR: Page Table does not exist.\n");
				return 0;
		}
	}
	else
	{
		//TODO: Implement this for other address spaces.
		return 0;
	}
	vaddr_t virtualAddress = indexToAddress(pdIndex, ptIndex);

	// Flush the TLB
	asm volatile ("invlpg (%0)" :: "r"(virtualAddress));
}



void AddressSpace::unmap(vaddr_t virtualAddress)
{
	mapAt(virtualAddress, 0, 0);
}
