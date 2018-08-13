#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/print.h>
#include <stddef.h> /* size_t */

static paddr_t* const stack = (paddr_t*) 0xFFC00000;
static size_t stackUsed = 0;
static size_t stackUnused = 0;

extern struct _incomplete_type bootstrapBegin;
extern struct _incomplete_type bootstrapEnd;
extern struct _incomplete_type kernelPhysicalBegin;
extern struct _incomplete_type kernelPhysicalEnd;

static int isAddressInUse(paddr_t physicalAddress)
{
    return (physicalAddress >= (paddr_t) &bootstrapBegin &&
            physicalAddress <= (paddr_t) &bootstrapEnd) ||
            (physicalAddress >= (paddr_t) &kernelPhysicalBegin &&
            physicalAddress <= (paddr_t) &kernelPhysicalEnd) ||
            physicalAddress == 0;
}

void Physicalmemory_initialize(struct multiboot_info* multiboot)
{
    paddr_t mmapPhys = (paddr_t) multiboot->mmap_addr;
    paddr_t mmapAligned = mmapPhys & ~0xFFF;
    ptrdiff_t offset = mmapPhys - mmapAligned;

    vaddr_t virtualAddress = kernelSpace->map(mmapAligned, 0x1 | 0x2);

    vaddr_t mmap = virtualAddress + offset;
    vaddr_t mmapEnd = mmap + multiboot->mmap_length;

    while (mmap < mmapEnd)
    {
        struct multiboot_mmap_entry* mmapEntry = (struct multiboot_mmap_entry*) mmap;
        if (mmapEntry->type == MULTIBOOT_MEMORY_AVAILABLE)
        {
            paddr_t addr = (paddr_t) mmapEntry->addr;
            for (uint64_t i = 0; i < mmapEntry->len; i+= 0x1000)
            {
                if (isAddressInUse(addr + i))
                    continue;
                Physicalmemory_pushPageFrame(addr + i);
            }
        }
        mmap += mmapEntry->size + 4;
    }
    kernelSpace->unmap(virtualAddress);
    printf("We have %u free page frames.\n", stackUsed);
}

void Physicalmemory_pushPageFrame(paddr_t physicalAddress)
{
    if (unlikely(stackUnused == 0))
    {
        kernelSpace->mapAt((vaddr_t) stack - 0x1000 - stackUsed * 4,
                physicalAddress, 0x1 | 0x2);
        stackUnused += 1024;
    }
    else
    {
        stack[-++stackUsed] = physicalAddress;
        stackUnused--;
    }
}
paddr_t Physicalmemory_popPageFrame()
{
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
            printf("Out of memory\n");
            return 0;
        }
    }
     else
    {
        stackUnused++;
        return stack[-stackUsed--];
    }
}