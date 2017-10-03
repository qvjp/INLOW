#include <stddef.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/kernel.h>

#define RECURSIVE_MAPPING 0xFFC00000

extern "C"
{
	extern symbol_t bootstrapBegin;
	extern symbol_t bootstrapEnd;
}

AddressSpace AddressSpace::_kernelSpace;
AddressSpace* kernelSpace;

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

vaddr_t AddressSpace::mapAt(vaddr_t virtualAddress, paddr_t physicalAddress, int flags)
{
	size_t pdIndex = virtualAddress >> 22;
	size_t ptIndex = (virtualAddress >> 12) & 0x3FF;

	if(this == kernelSpace)
	{
		uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);
		*pageEntry = physicalAddress | flags;
	}
	else
	{
		//TODO: Implement this for other address spaces.
	}

	// Flush the TLB
	asm volatile ("invlpg (%0)" :: "r"(virtualAddress));

	return virtualAddress;
}

void AddressSpace::unmap(vaddr_t virtualAddress)
{
	mapAt(virtualAddress, 0, 0);
}
