#include <assert.h>
#include <stdio.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>

static paddr_t* const stack = (paddr_t*) 0xFFC00000;
static size_t stackUsed = 0;
static size_t stackUnused = 0;

extern "C"
{
		extern symbol_t bootstrapBegin;
		extern symbol_t bootstrapEnd;
		extern symbol_t kernelPhysicalBegin;
		extern symbol_t kernelPhysicalEnd;
}

static inline bool isAddressInUse(paddr_t physicalAddress)
{
		return (physicalAddress >= (paddr_t) &bootstrapBegin &&
				physicalAddress <= (paddr_t) &bootstrapEnd) ||
				(physicalAddress >= (paddr_t) &kernelPhysicalBegin &&
				 physicalAddress <= (paddr_t) &kernelPhysicalEnd) ||
				physicalAddress == 0;
}

void PhysicalMemory::initialize(multiboot_info* multiboot)
{
	paddr_t mmapPhys = (paddr_t)multiboot->mmap_addr;
	paddr_t mmapAligned = mmapPhys & ~0xFFF;
	ptrdiff_t offset = mmapPhys - mmapAligned;

	vaddr_t virtualAddress = kernelSpace->mapPhysical(mmapAligned, 0x1000, PROT_READ);
	vaddr_t mmap = virtualAddress + offset;
	vaddr_t mmapEnd = mmap + multiboot->mmap_length;

	while(mmap < mmapEnd)
	{
		multiboot_mmap_entry* mmapEntry = (multiboot_mmap_entry*) mmap;

		if (mmapEntry->type == MULTIBOOT_MEMORY_AVAILABLE && mmapEntry->addr + mmapEntry->len <= UINTPTR_MAX)
		{
			paddr_t addr = (paddr_t) mmapEntry->addr;
			for (uint64_t i = 0; i < mmapEntry->len; i += 0x1000)
			{
				if (isAddressInUse(addr + i))
						continue;
				pushPageFrame(addr + i);
			}
		}
		mmap += mmapEntry->size + 4;
	}

	kernelSpace->unmapPhysical(virtualAddress, 0x1000);
	Print::printf("Free Memory: %zu KiB\n", stackUsed * 4);
}


void PhysicalMemory::pushPageFrame(paddr_t physicalAddress)
{
		assert(!(physicalAddress & 0xFFF));
	if(unlikely(stackUnused == 0))
	{
		kernelSpace->mapPhysical((vaddr_t) stack - 0x1000 - stackUsed * 4, physicalAddress, 0x1000, PROT_READ | PROT_WRITE);
		stackUnused += 1024;
	}
	else
	{
		stack[-++stackUsed] = physicalAddress;
		stackUnused--;
	}
}


paddr_t PhysicalMemory::popPageFrame()
{
	if (unlikely(stackUsed == 0))
	{
		if (likely(stackUnused > 0))
		{
			vaddr_t virt = (vaddr_t) stack - stackUnused * 4;
			paddr_t result = kernelSpace->getPhysicalAddress(virt);
			kernelSpace->unmapPhysical(virt, 0x1000);
			stackUnused -= 1024;
			return result;
		}
		else
		{
			Print::printf("Out of memory\n");
			return 0;
		}
	}
	else
	{
		stackUnused++;
		return stack[-stackUsed--];
	}

}
