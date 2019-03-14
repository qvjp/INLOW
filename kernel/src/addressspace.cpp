/** MIT License
 * 
 * Copyright (c) 2019 INLOW-OS
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * kernel/src/addressspace.cpp
 * 实现地址空间的基本操作
 */


#include <inlow/kernel/addressspace.h>
#include <inlow/kernel/physicalmemory.h> /* pushPageFrame popPageFrame */           
#include <inlow/kernel/print.h>          /* Print::printf() */
#include <stdlib.h>                      /* malloc() */
#include <string.h>                      /* memset() */


AddressSpace AddressSpace::_kernelSpace;
AddressSpace* kernelSpace;

static AddressSpace* firstAddressSpace = nullptr;

/**
 * 每个进程对应一个地址空间，地址空间包含两部分
 * 所使用的页目录和指向下一个地址空间的指针。
 */
AddressSpace::AddressSpace()
{
    pageDir = 0;
    next = nullptr;
}

/**
 * 通过页目录和页表号找到对应的虚拟地址
 */
static inline inlow_vir_addr_t offset2address(size_t pdOffset, size_t ptOffset)
{
    return (pdOffset << 22) | (ptOffset << 12);
}

/**
 * 虚拟地址转换到对应的页目录号和页表号
 */
static inline void address2offset(inlow_vir_addr_t virtualAddress, size_t& pdOffset, size_t& ptOffset)
{
    /* 高10位是页目录偏移 */
    pdOffset = virtualAddress >> 22;
    /* 中间10位是页表偏移 */
    ptOffset = (virtualAddress >> 12) & 0x3FF;
}

/**
 * 初始化地址空间
 * 在开启分页时曾经将内核虚拟地址映射到其物理地址（identity mapping）
 * 分页开启后这个映射不再需要，取消这个映射。
 * 
 */
void AddressSpace::initialize()
{
    kernelSpace = &_kernelSpace;
    kernelSpace->pageDir = (inlow_phy_addr_t) &kernelPageDirectory;
    inlow_vir_addr_t p = (inlow_vir_addr_t) &bootstrapBegin;

    while (p < (inlow_vir_addr_t) &bootstrapEnd)
    {
        kernelSpace->unMap(p);
        p += 0x1000;
    }
    /**
     * 可以将下边这个映射干掉是因为第一个页表对应的内存是已知的最高4M。
     */
    kernelSpace->unMap(RECURSIVE_MAPPING);
}

/**
 * 将当前地址空间激活
 * 也就是将当前的页目录设置给%cr3
 */
void AddressSpace::activate()
{
    __asm__ __volatile__ ("mov %0, %%cr3" :: "r"(pageDir));
}

/**
 * 分配指向pages_number个页大小的虚拟地址
 */
inlow_vir_addr_t AddressSpace::allocate(size_t pages_number)
{
    inlow_phy_addr_t physicalAddresses[pages_number + 1];
    for (size_t i = 0; i < pages_number; i++)
    {
        physicalAddresses[i] = PhysicalMemory::popPageFrame();
        if (!physicalAddresses[i])
            return 0;
    }
    physicalAddresses[pages_number] = 0;
    int flags = PAGE_PRESENT | PAGE_WRITABLE;
    if (this != kernelSpace)
        flags |= PAGE_USER;
    return mapRange(physicalAddresses, flags);
}

/**
 * 启动新进程时，fork前一个进程的地址空间
 */
AddressSpace* AddressSpace::fork()
{
    AddressSpace* result = (AddressSpace*)malloc(sizeof(AddressSpace));
    result->pageDir = PhysicalMemory::popPageFrame();

    inlow_vir_addr_t currentPageDir = kernelSpace->map(pageDir, 0x1);
    inlow_vir_addr_t newPageDir = kernelSpace->map(result->pageDir, 0x3);

    memcpy((void*) newPageDir, (const void*) currentPageDir, 0x1000);

    kernelSpace->unMap(currentPageDir);
    kernelSpace->unMap(newPageDir);
    result->next = firstAddressSpace;
    firstAddressSpace = result;

    return result;
}

/**
 * 将virtualAddress开始的pages_number大小（单位字节）的虚拟内存释放
 */
void AddressSpace::free(inlow_vir_addr_t virtualAddress, size_t pages_number)
{
    for (size_t i = 0; i < pages_number; i++)
    {
        inlow_phy_addr_t physicalAddress = getPhysicalAddress(virtualAddress);
        unMap(virtualAddress);
        PhysicalMemory::pushPageFrame(physicalAddress);
        virtualAddress += 0x1000;
    }
}

/**
 * 获取虚拟地址对应的物理地址
 */
inlow_phy_addr_t AddressSpace::getPhysicalAddress(inlow_vir_addr_t virtualAddress)
{
    size_t pdOffset;
    size_t ptOffset;
    address2offset(virtualAddress, pdOffset, ptOffset);

    uintptr_t* pageDirectory;
    uintptr_t* pageTable = nullptr;
    inlow_phy_addr_t result = 0;
    if (this == kernelSpace)
    {
        /* 内核态页目录从RECURSIVE_MAPPING + 0x3FF000开始 */
        pageDirectory = (uintptr_t*)(RECURSIVE_MAPPING + 0x3FF000);
        pageTable = (uintptr_t*) (RECURSIVE_MAPPING + 0x1000 * pdOffset);
    }
    else
    {
        pageDirectory = (uintptr_t*) kernelSpace->map(pageDir,0x1);
    }
    if (pageDirectory[pdOffset])
    {
        if (this != kernelSpace)
        {
            pageTable = (uintptr_t*) kernelSpace->map(pageDirectory[pdOffset] & ~0xFFF, 0x1);
        }
        result = pageTable[ptOffset] & ~0xFFF;
    }
    if (this != kernelSpace)
    {
        if (pageTable) kernelSpace->unMap((inlow_vir_addr_t) pageTable);
        kernelSpace->unMap((inlow_vir_addr_t) pageDirectory);
    }
    return result;
}


/**
 * 判断所给虚拟地址是否是空闲
 */
bool AddressSpace::isFree(inlow_vir_addr_t virtualAddress)
{
    size_t pdOffset;
    size_t ptOffset;
    address2offset(virtualAddress, pdOffset, ptOffset);
    return isFree(pdOffset, ptOffset);
}

/**
 * 判断所给页目录页表所指内存页是否空闲
 */
bool AddressSpace::isFree(size_t pdOffset, size_t ptOffset)
{
    if (pdOffset == 0 && ptOffset == 0)
        return false;
    uintptr_t* pageDirectory;
    uintptr_t* pageTable = nullptr;
    bool result;

    if (this == kernelSpace)
    {
        pageDirectory = (uintptr_t*) (RECURSIVE_MAPPING + 0x3FF000);
        pageTable = (uintptr_t*) (RECURSIVE_MAPPING + 0x1000 * pdOffset);
    }
    else
    {
        pageDirectory = (uintptr_t*) kernelSpace->map(pageDir, PAGE_PRESENT);
    }

    if (!pageDirectory[pdOffset])
        result = true;
    else
    {
        if (this != kernelSpace)
            pageTable = (uintptr_t*) kernelSpace->map(pageDirectory[pdOffset] & ~0xFFF, PAGE_PRESENT);
    
        result = !pageTable[ptOffset];
    }

    if (this != kernelSpace)
    {
        if (pageTable)
            kernelSpace->unMap((inlow_vir_addr_t) pageTable);
        kernelSpace->unMap((inlow_vir_addr_t) pageDirectory);
    }
    return result;
}

/**
 * 将物理地址映射到虚拟地址
 * flags 0x3是可读可写，0x1是只读，0x0不可用
 */
inlow_vir_addr_t AddressSpace::map(inlow_phy_addr_t physicalAddress, uint16_t flags)
{
    size_t begin;
    size_t end;
    if (this == kernelSpace)
    {
        begin = 0x300;
        end = 0x400;
    }
    else
    {
        begin = 0;
        end = 0x300;
    }
    
    /* 找到没有映射的页，并映射 */
    for (size_t pdOffset = begin; pdOffset < end; pdOffset++)
    {
        for (size_t ptOffset = 0; ptOffset < 0x400; ptOffset++)
        {
            if (isFree(pdOffset, ptOffset))
            {
                return mapAt(pdOffset, ptOffset, physicalAddress, flags);
            }
        }
    }
    return 0;
}

/**
 * 下边两个函数是将虚拟地址映射和物理地址映射起来
 * 分别通过具体的虚拟地址和通过PDE、PTE
 * flags和map函数一样，都是0x3是可读可写，0x1是只读，0x0不可用
 */
inlow_vir_addr_t AddressSpace::mapAt(inlow_vir_addr_t virtualAddress, inlow_phy_addr_t physicalAddress, uint16_t flags)
{
    size_t pdOffset;
    size_t ptOffset;
    address2offset(virtualAddress, pdOffset, ptOffset);
    return mapAt(pdOffset, ptOffset, physicalAddress, flags);
}

inlow_vir_addr_t AddressSpace::mapAt(size_t pdOffset, size_t ptOffset, inlow_phy_addr_t physicalAddress, uint16_t flags)
{
    uintptr_t* pageDirectory;
    uintptr_t* pageTable = nullptr;

    if (this == kernelSpace) {
        /* 页目录从0xFFFFF000（RECURSIVE_MAPPING+0x3FF000）开始 */
        pageDirectory = (uintptr_t*) (RECURSIVE_MAPPING + 0x3FF000);
        pageTable = (uintptr_t*) (RECURSIVE_MAPPING + 0x1000 * pdOffset);
    }
    else
    {
        pageDirectory = (uintptr_t*) kernelSpace->map(pageDir, 0x3);
    }

    if (!pageDirectory[pdOffset])
    {
        inlow_phy_addr_t pageTablePhys = PhysicalMemory::popPageFrame();
        int pdFlags = PAGE_PRESENT | PAGE_WRITABLE;
        if (this != kernelSpace)
            pdFlags |= PAGE_USER;
        pageDirectory[pdOffset] = pageTablePhys | pdFlags;

        if (this != kernelSpace)
        {
            pageTable = (uintptr_t*) kernelSpace->map(pageTablePhys, 0x3);
        }
        memset(pageTable, 0, 0x1000);

        if (this == kernelSpace)
        {
            AddressSpace* addressSpace = firstAddressSpace;
            while (addressSpace)
            {
                uintptr_t* pageDir = (uintptr_t*) map(addressSpace->pageDir, 0x3);
                pageDir[pdOffset] = pageTablePhys | 0x3;
                unMap((inlow_vir_addr_t) pageDir);
                addressSpace = addressSpace->next;
            }
        }
    }
    else if (this != kernelSpace)
    {
        pageTable = (uintptr_t*) kernelSpace->map(pageDirectory[pdOffset] & ~0xFFF, 0x3);
    }
    pageTable[ptOffset] = physicalAddress | flags;

    if (this != kernelSpace)
    {
        kernelSpace->unMap((inlow_vir_addr_t) pageTable);
        kernelSpace->unMap((inlow_vir_addr_t) pageDirectory);
    }
    inlow_vir_addr_t virtualAddress = offset2address(pdOffset, ptOffset);

    /* 由于虚拟地址指向的物理地址改变了，所以要更新TLB */
    __asm__ __volatile__ ("invlpg (%0)" :: "r"(virtualAddress));

    return virtualAddress;
}

/**
 * 将一串物理地址（不一定连续）映射到 连续 的虚拟地址
 * 然后将这个虚拟地址首地址返回
 */
inlow_vir_addr_t AddressSpace::mapRange(inlow_phy_addr_t* physicalAddresses, uint16_t flags)
{
    inlow_phy_addr_t* phys = physicalAddresses;
    size_t pages_number = 0;
    /* 找到一共有多少个物理页要处理 */
    while (*phys++)
        pages_number++;

    size_t begin;
    size_t end;

    if (this == kernelSpace)
    {
        begin = 0x300;
        end = 0x400;
    }
    else
    {
        begin = 0;
        end = 0x300;
    }
    

    /* 在高地址空间找到足够的连续页 */
    for (size_t pdOffset = begin; pdOffset < end; pdOffset++)
    {
        for (size_t ptOffset = 0; ptOffset < 0x400; ptOffset++)
        {
            if (pdOffset == 0 && ptOffset == 0)
                continue;
            size_t pd = pdOffset;
            size_t pt = ptOffset;
            size_t foundPages = 0;

            while (foundPages <= pages_number)
            {
                if (!isFree(pd, pt))
                    break;
                pt++;
                foundPages++;
                if (pt >= 1024)
                {
                    pd++;
                    pt = 0;
                }
            }
            if (foundPages >= pages_number)
                return mapRangeAt(offset2address(pdOffset, ptOffset), physicalAddresses, flags);
        }
    }
    return 0;
}

/**
 * 将指定大小的物理空间映射到虚拟空间
 */
inlow_vir_addr_t AddressSpace::mapRange(inlow_phy_addr_t firstPhysicalAddress, size_t pages_number, uint16_t flags)
{
    inlow_phy_addr_t physicalAddresses[pages_number + 1];
    for (size_t i = 0; i < pages_number; i++)
    {
        physicalAddresses[i] = firstPhysicalAddress;
        firstPhysicalAddress += 0x1000;
    }
    physicalAddresses[pages_number] = 0;

    return mapRange(physicalAddresses, flags);
}

/**
 * 将传入的物理地址和虚拟地址映射起来
 * 判断物理地址结束标志是通过在物理地址最后设置一个结束标志0
 */
inlow_vir_addr_t AddressSpace::mapRangeAt(inlow_vir_addr_t virtualAddress, inlow_phy_addr_t* physicalAddresses, uint16_t flags)
{
    inlow_vir_addr_t addr = virtualAddress;
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
/**
 * 取消虚拟地址到物理地址映射，通过将其映射到物理地址0，并将flags设为0.
 */
void AddressSpace::unMap(inlow_vir_addr_t virtualAddress)
{
    mapAt(virtualAddress, 0, 0);
}

/**
 * 将指定大小的虚拟地址需要映射
 */
void AddressSpace::unMapRange(inlow_vir_addr_t firstVirtualAddress, size_t pages_number)
{
    while (pages_number--)
    {
        unMap(firstVirtualAddress);
        firstVirtualAddress += 0x1000;
    }
}

/**
 * 下边两个函数供libk调用
 * 分配/释放大小为pages_number个页的连续内存
 */
extern "C" void* __mapPages(size_t pages_number)
{
    return (void*) kernelSpace->allocate(pages_number);
}
extern "C" void __unmapPages(void* addr, size_t pages_number)
{
    kernelSpace->free((inlow_vir_addr_t) addr, pages_number);
}
