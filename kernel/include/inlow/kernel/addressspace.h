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
 * kernel/include/inlow/kernel/addressspace.h
 * 定义AddressSpace类
 */

#ifndef KERNEL_ADDRESSSPACE_H__
#define KERNEL_ADDRESSSPACE_H__

#include <stddef.h> /* size_t */
#include <inlow/kernel/inlow.h> /* inlow_phy_addr_t inlow_vir_addr_t */

#define RECURSIVE_MAPPING 0xFFC00000

/**
 * 获取linker.ld中
 * bootstapBegin
 * bootstrapEnd
 * kernelPhysicalBegin
 * kernelPhysicalEnd
 * 的地址
 */
extern "C"
{
    void bootstrapBegin();
    void bootstrapEnd();
    void kernelPhysicalBegin();
    void kernelPhysicalEnd();
}

/**
 * 地址空间类
 */ 
class AddressSpace
{
    public:
        inlow_phy_addr_t getPhysicalAddress(inlow_vir_addr_t virtualAddress);

        inlow_vir_addr_t map(inlow_phy_addr_t physicalAddress, uint16_t flags);
        inlow_vir_addr_t mapAt(inlow_vir_addr_t virtualAddress, inlow_phy_addr_t physicalAddress, uint16_t flags);
        inlow_vir_addr_t mapAt(size_t pdIndex, size_t ptIndex, inlow_phy_addr_t physicalAddress, uint16_t flags);

        void unMap(inlow_vir_addr_t virtualAddress);
        static void initialize();
    private:
        AddressSpace();
        static AddressSpace _kernelSpace;
};
/* 将内核地址空间设置为全局变量 */
extern AddressSpace* kernelSpace;

#endif /* KERNEL_ADDRESSSPACE_H__ */
