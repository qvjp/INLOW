#include <assert.h>
#include <stdio.h>
#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/kthread.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/physicalmemory.h>

static paddr_t* const stack = (paddr_t*) 0xFFC00000;
static size_t stackUsed = 0;
static size_t stackUnused = 0;

static kthread_mutex_t mutex = KTHREAD_MUTEX_INITIALIZER;

extern "C"
{
		extern symbol_t bootstrapBegin;
		extern symbol_t bootstrapEnd;
		extern symbol_t kernelPhysicalBegin;
		extern symbol_t kernelPhysicalEnd;
}

static inline bool isUsedByKernel(paddr_t physicalAddress)
{
		return (physicalAddress >= (paddr_t) &bootstrapBegin &&
				physicalAddress < (paddr_t) &bootstrapEnd) ||
				(physicalAddress >= (paddr_t) &kernelPhysicalBegin &&
				 physicalAddress < (paddr_t) &kernelPhysicalEnd) ||
				physicalAddress == 0;
}

static inline bool isUsedByModule(paddr_t physicalAddress, multiboot_mod_list* modules, uint32_t moduleCount)
{
	for (size_t i = 0; i < moduleCount; i++)
	{
		if (physicalAddress >= modules[i].mod_start && physicalAddress < modules[i].mod_end)
		{
			return true;
		}
	}
	return false;
}

static inline bool isUsedByMultiboot(paddr_t physicalAddress, multiboot_info* multiboot)
{
	paddr_t mmapEnd = multiboot->mmap_addr + multiboot->mmap_length;
	paddr_t modsEnd = multiboot->mods_addr + multiboot->mods_count * sizeof(multiboot_mod_list);
	return ((physicalAddress >= (multiboot->mmap_addr & ~0xFFF) && physicalAddress < mmapEnd) ||
			(physicalAddress >= (multiboot->mods_addr & ~0xFFF) && physicalAddress < modsEnd));
}

void PhysicalMemory::initialize(multiboot_info* multiboot)
{
	paddr_t mmapPhys = (paddr_t)multiboot->mmap_addr;
	paddr_t mmapAligned = mmapPhys & ~0xFFF;
	ptrdiff_t mmapOffset = mmapPhys - mmapAligned;
	size_t mmapSize = ALIGNUP(mmapOffset + multiboot->mmap_length, 0x1000);

	paddr_t modulesPhys = (paddr_t) multiboot->mods_addr;
	paddr_t modulesAligned = modulesPhys & ~0xFFF;
	ptrdiff_t modulesOffset = modulesPhys - modulesAligned;
	size_t modulesSize = ALIGNUP(modulesOffset + multiboot->mods_count * sizeof(multiboot_mod_list), 0x1000);
	vaddr_t mmapMapped = kernelSpace->mapPhysical(mmapAligned, mmapSize, PROT_READ);
	vaddr_t modulesMapped = kernelSpace->mapPhysical(modulesAligned, modulesSize, PROT_READ);

	vaddr_t mmap = mmapMapped + mmapOffset;
	vaddr_t mmapEnd = mmap + multiboot->mmap_length;

	multiboot_mod_list* modules = (multiboot_mod_list*) (modulesMapped + modulesOffset);

	while(mmap < mmapEnd)
	{
		multiboot_mmap_entry* mmapEntry = (multiboot_mmap_entry*) mmap;

		if (mmapEntry->type == MULTIBOOT_MEMORY_AVAILABLE && mmapEntry->addr + mmapEntry->len <= UINTPTR_MAX)
		{
			paddr_t addr = (paddr_t) mmapEntry->addr;
			for (uint64_t i = 0; i < mmapEntry->len; i += 0x1000)
			{
				if (isUsedByModule(addr + i, modules, multiboot->mods_count) ||
						isUsedByKernel(addr + i) ||
						isUsedByMultiboot(addr + i, multiboot))
				{
					continue;
				}
				pushPageFrame(addr + i);
			}
		}
		mmap += mmapEntry->size + 4;
	}

	kernelSpace->unmapPhysical(mmapMapped, mmapSize);
	kernelSpace->unmapPhysical(modulesMapped, modulesSize);
	// Print::printf("Free Memory: %zu KiB\n", stackUsed * 4);
}


void PhysicalMemory::pushPageFrame(paddr_t physicalAddress)
{
	assert(!(physicalAddress & 0xFFF));
	AutoLock lock(&mutex);
	if(unlikely(stackUnused == 0))
	{
		kernelSpace->mapAt((vaddr_t) stack - 0x1000 - stackUsed * 4, physicalAddress, PROT_READ | PROT_WRITE);
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
	AutoLock lock(&mutex);
	if (unlikely(stackUsed == 0))
	{
		if (likely(stackUnused > 0))
		{
			vaddr_t virt = (vaddr_t) stack - stackUnused * 4;
			paddr_t result = kernelSpace->getPhysicalAddress(virt);
			kernelSpace->unmap(virt);
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
