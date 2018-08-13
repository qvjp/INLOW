#include <inlow/kernel/print.h> /* printf() */
#include <inlow/kernel/addressspace.h>
#include <stddef.h> /* size_t */
#include <stdarg.h> /* va_list va_start() va_arg() va_end() */

#define RECURSIVE_MAPPING 0xFFC00000

/**
 * 使用_incomplete_type可以获得它们的地址
 */
extern struct _incomplete_type bootstrapBegin;
extern struct _incomplete_type bootstrapEnd;

AddressSpace* kernelSpace;

/**
 * 初始化方法
 * 绑定函数指针
 */
AddressSpace* new_AddressSpace(AddressSpace* this)
{
    this->mapAt = AddressSpace_mapAt;
    this->unmap = AddressSpace_unmap;
    this->initialize = AddressSpace_initialize;
    this->getPhysicalAddress = AddressSpace_getPhysicalAddress;
    return this;
}

/**
 * 下边是线性地址到物理地址的转换方式
 * 下边三个方法
 * indexToAddress()
 * addressToPdIndex()
 * addressToPtIndex()
 * 实现了这一转换
 * Linear address:
 * |31               22|21               12|11                    0|
 * -----------------------------------------------------------------
 * |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
 * --------------------|-------------------|------------------------
 * \________  ________/ \________  _______/ \__________  __________/
 *          \/                   \/                    \/
 *           |                    |                     |
 *           / 10                 / 10                  / 12
 *           |                    |                     |  page frame
 *           | page directory     |                     |  ----------
 *           |   ----------       |   page table        |  |        |
 *           |   |    .   |       |   ----------        |  |    .   |
 *           |   |    .   |       |   |    .   |        |  |    .   |
 *           |   |    .   |       |   |    .   |        |  |    .   |
 *           |   |--------|       |   |    .   |        |  |        |
 *           |   |PD entry|__     |   |--------|        |  |--------|
 *           --> |--------|  |    |   |PT entry|--      |  |phy addr|
 *               |    .   |  |    --> |--------|  |     -> |--------|
 *               |    .   |  / 20     |    .   |  |        |        |
 *               |    .   |  |        |    .   |  |        |    .   |
 *         ----> ----------  |        |    .   |  / 20     |    .   |
 *         / 20              -------> ----------  |        |    .   |
 *         |     |--------|                       |        |        |
 *         ------|  CR3   |                       |        |        |
 *               |--------|                       -------> ----------
 */
static vaddr_t indexToAddress(size_t pdIndex, size_t ptIndex)
{
    return (pdIndex << 22) | (ptIndex << 12);
}
static size_t addressToPdIndex(vaddr_t virtualAddress)
{
    return virtualAddress >> 22;
}
static size_t addressToPtIndex(vaddr_t virtualAddress)
{
    // 0x3FF是二进制0000 0011 1111 1111，也就是只取virtualAddress[12-21]位上的数据
    return(virtualAddress >> 12) & 0x3FF;
}

vaddr_t AddressSpace_map(paddr_t physicalAddress, int flags)
{
    // 在高地址空间找到空闲页并map
    for (size_t pdIndex = 0x300; pdIndex < 0x400; pdIndex++)
    {
        for (size_t ptIndex = 0; ptIndex < 0x400; ptIndex++)
        {
            uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);
            if (!pageEntry)
            {
                return AddressSpace_mapAt(pdIndex, ptIndex, physicalAddress, flags);
            }
        }
    }
    return 0;
}

/**
 * 线性地址到物理地址的映射函数，
 * 这里使用了C语言的不定参数特性，当然可以写两个函数
 * 没有别的特殊原因，就是好玩。
 * 可以往这个函数传两种参数，第一个参数type是1或2，分别对应下边的参数列表：
 * vaddr_t AddressSpace_mapAt(vaddr_t virtualAddress, paddr_t physicalAddress, int flags); // 1
 * vaddr_t AddressSpace_mapAt(size_t pdIndex, size_t ptIndex, paddr_t physicalAddress, int flags); // 2
 * 
 * type == 1时，是虚拟地址到物理地址的映射
 * type == 2时，是页目录+页表到物理地址的映射
 */
vaddr_t AddressSpace_mapAt(int type, ...)
{
    va_list ap;
    va_start(ap, type);
    // 如果是虚拟地址直接映射物理地址，先找到页目录和页表，再调用type == 2的方法
    if (type == 1)
    {
        vaddr_t virtualAddress = va_arg(ap, vaddr_t);
        paddr_t physicalAddress = va_arg(ap, paddr_t);
        int flags = va_arg(ap, int);

        size_t pdIndex = addressToPdIndex(virtualAddress);
        size_t ptIndex = addressToPtIndex(virtualAddress);
        // printf("pdIndex: %d ptIndex: %d\n", pdIndex, ptIndex);
        return AddressSpace_mapAt(2, pdIndex, ptIndex, physicalAddress, flags);
    }
    if (type == 2)
    {
        size_t pdIndex = va_arg(ap, size_t);
        size_t ptIndex = va_arg(ap, size_t);
        paddr_t physicalAddress = va_arg(ap, paddr_t);
        int flags = va_arg(ap, int);
        printf("pdIndex: %d ptIndex: %d\n", pdIndex, ptIndex);
        uintptr_t* pageTable = (uintptr_t*)(RECURSIVE_MAPPING + 0x3FF000 + 4 * pdIndex);
        printf("pageTable: 0x%x\n", pageTable);
        if (*pageTable)
        {
            uintptr_t* pageEntry = (uintptr_t*)(RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);
            *pageEntry = physicalAddress | flags;
            printf("pageEntry: 0x%x\n", pageEntry);
            printf("*pageEntry: %d\n", *pageEntry);
        }
        else
        {
            // TODO: 分配新页表
            printf("Error: Page Table does not exist!\n");
            return 0;
        }
        vaddr_t virtualAddress = indexToAddress(pdIndex, ptIndex);

        // 刷新TLB
        asm volatile ("invlpg (%0)" :: "r"(virtualAddress));
        return virtualAddress;
    }
    else
    {
        // 其他地址空间
        return 0;
    }
    va_end(ap);
    return 0;
}

/**
 * 取消指定虚拟地址到物理地址的绑定
 * 是将此虚拟地址映射到物理地址0
 */
void AddressSpace_unmap(vaddr_t virtualAddress)
{
    AddressSpace_mapAt(1, virtualAddress, 0, 0);
}

/**
 * 初始化地址空间包括调用“构造方法”，实现函数帮定
 * 和取消映射启动部分空间
 */
void AddressSpace_initialize()
{
    AddressSpace space;
    kernelSpace =  new_AddressSpace(&space);
    vaddr_t p = (vaddr_t) &bootstrapBegin;
    while (p < (vaddr_t) &bootstrapEnd)
    {
        kernelSpace->unmap(p);
        p += 0x1000;
    }
    printf("AddressSpace_initialize: 0x%x\n", kernelSpace);
}

paddr_t AddressSpace_getPhysicalAddress(vaddr_t virtualAddress)
{
    size_t pdIndex = addressToPdIndex(virtualAddress);
    size_t ptIndex = addressToPtIndex(virtualAddress);

    uintptr_t* pageTable = (uintptr_t*) (RECURSIVE_MAPPING + 0x3FF000 + 4 * pdIndex);
    if (*pageTable)
    {
        uintptr_t* pageEntry = (uintptr_t*)
                (RECURSIVE_MAPPING + 0x1000 * pdIndex + 4 * ptIndex);
            return *pageEntry & ~0xFFF;
    }
    else
    {
        printf("Error: Page Table does exist.\n");
        return 0;
    }
}