#include <string.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/kernel.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>

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

vaddr_t AddressSpace::allocate(size_t nPages)
{
		paddr_t physicalAddresses[nPages + 1];

		for (size_t i = 0; i < nPages; i++)
		{
				physicalAddresses[i] = PhysicalMemory::popPageFrame();
				if (!physicalAddresses[i])
						return 0;
		}
		physicalAddresses[nPages] = 0;
		return mapRange(physicalAddresses, PAGE_PRESENT | PAGE_WRITABLE);
}

void AddressSpace::free(vaddr_t virtualAddress, size_t nPages)
{
		for (size_t i = 0; i < nPages; i++)
		{
				paddr_t physicalAddress = getPhysicalAddress(virtualAddress);
				unmap(virtualAddress);
				PhysicalMemory::pushPageFrame(physicalAddress);
				virtualAddress += 0x1000;
		}
}

paddr_t AddressSpace::getPhysicalAddress(vaddr_t virtualAddress)
{
		size_t pdIndex;
		size_t ptIndex;
		addressToIndex(virtualAddress, pdIndex, ptIndex);

		if(this == kernelSpace)
		{
				uintptr_t* pageTable = (uintptr_t*)(RECURSIVE_MAPPING + 0x3FF000 + 4 * pdIndex);
				if (*pageTable)
				{
						uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);
						return *pageEntry & ~0xFFF;
				}
				else
				{
						Print::printf("Error: Page Table does not exist.\n");
						return 0;
				}
		}
		else
		{
				//TODO: Implement this for other address spaces.
				return 0;	
		}
}
vaddr_t AddressSpace::map(paddr_t physicalAddress, int flags)
{
		// Find free page from the higher half amd map it
		for (size_t pdIndex = 0x300; pdIndex < 0x400; pdIndex++)
		{
				uintptr_t* pageTable = (uintptr_t*)(RECURSIVE_MAPPING + 0x3FF000 + 4 * pdIndex);
				if (!*pageTable)
						continue;
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
				uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);

				if (!*pageTable)
						//Allocate a new page table and map it in the page directory
				{
						paddr_t pageTablePhys = PhysicalMemory::popPageFrame();
						uintptr_t* pageTableVirt = (uintptr_t*) mapAt(0x3FF, pdIndex, pageTablePhys, PAGE_PRESENT | PAGE_WRITABLE);
						memset(pageTableVirt, 0, 0x1000);
				}
				*pageEntry = physicalAddress | flags;

		}
		else
		{
				//TODO: Implement this for other address spaces.
				return 0;
		}
		vaddr_t virtualAddress = indexToAddress(pdIndex, ptIndex);

		// Flush the TLB
		asm volatile ("invlpg (%0)" :: "r"(virtualAddress));
		return virtualAddress;
}
vaddr_t AddressSpace::mapRange(paddr_t* physicalAddresses, int flags)
{
		paddr_t* phys = physicalAddresses;
		size_t nPages = 0;

		while (*phys++)
		{
				nPages++;
		}

		for (size_t pdIndex = 0x300; pdIndex < 0x400; pdIndex++)
		{
				for (size_t ptIndex = 0; ptIndex < 0x400; ptIndex++)
				{
						size_t pd = pdIndex;
						size_t pt = ptIndex;
						size_t foundPages = 0;

						while (foundPages <= nPages)
						{
								uintptr_t* pageTable = (uintptr_t*)(RECURSIVE_MAPPING + 0x3FF000 + 4 * pd);
								uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pd + 4 * pt);

								if (!*pageTable)
								{
										foundPages += 1024;
										pd++;
										pt = 0;
										continue;
								}

								if(*pageEntry)
										break;

								pt++;
								foundPages++;

								if(pt >= 1024)
								{
										pd++;
										pt = 0;
								}
						}
						if (foundPages >= nPages)
						{
								return mapRangeAt(indexToAddress(pdIndex, ptIndex),physicalAddresses, flags);
						}
				}
		}
		return 0;
}

vaddr_t AddressSpace::mapRangeAt(vaddr_t virtualAddress, paddr_t* physicalAddresses, int flags)
{
	vaddr_t addr = virtualAddress;
	while (*physicalAddresses)
	{
		if (!mapAt(addr, *physicalAddresses, flags))
		{
			return 0;
		}
		addr += 0x1000;
		physicalAddresses++;
	}
	return virtualAddress;
}

void AddressSpace::unmap(vaddr_t virtualAddress)
{
		mapAt(virtualAddress, 0, 0);
}

// These two function are called from libk
extern "C" void* __mapPages(size_t nPages)
{
	return (void*) kernelSpace->allocate(nPages);
}

extern "C" void __unmapPages(void* addr, size_t nPages)
{
	kernelSpace->free((vaddr_t) addr, nPages);
}
