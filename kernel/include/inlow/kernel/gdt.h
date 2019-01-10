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
 * kernel/include/inlow/kernel/gdt.h
 * 定义GDT
 */

#ifndef KERNEL_GDT_H__
#define KERNEL_GDT_H__

#include <stdint.h> /* uint16_t uint8_t */


/**
 *                             * GDT ENTRT *
 * |-------------------------------------------------------------------------|
 * |         |     |     |        |        |                |                |
 * |  Base   |Flags|Limit| Access |  Base  |      Base      |      Limit     |
 * |  24:31  |     |16:19|        | 16:23  |      0:15      |      0:15      |
 * |         |     |     |        |        |                |                |
 * |-------------------------------------------------------------------------|
 * |63     56|55 52|51 48|47    40|39    32|31            16|15             0| 
 * Limit: 20 bit 最大地址单元
 * Base: 32 bit  段开始处的线性地址
 * Access: 8 bit
 * Flags: 4 bit
 *             * Access Byte *                                 * Flags *
 * |-------------------------------------|               |-------------------|
 * |    |       |    |    |    |    |    |               |    |    |    |    |
 * | Pr | Privl | DT | Ex | DC | RW | AC |               | Gr | Sz | 0  | 0  |
 * |    |       |    |    |    |    |    |               |    |    |    |    |
 * |-------------------------------------|               |-------------------|
 * |    7       5    4    3    2    1   0|               |    7    6    5   4|
 * Pr: Present bit, 1 bit 所有有效的selectors这一位都设置为1
 * Privl: Privilege bit, 2 bit 0 = highest(kernel), 3 = lowest(user applications)
 * DT: Descriptor Type, 设置为1
 * Ex: Executable bit, 1是代码, 0是数据
 * DC: Direction bit/Conforming bit,
 *     对数据选择子是方向位，当DC = 0, 段向上增长, DC = 1, 段向下增长
 *     对代码选择子是位，当DC = 1, 允许设定的或更低权限执行, DC = 0, 只能设定的权限等级执行
 * RW: Readable bit/Writable bit,
 *     代码段可读，数据段可写
 *     并且代码段永远不可写，数据段永远可读
 * AC: Accessed bit, 设置为0即可，CPU在访问后会设置此位为1
 * Gr: Granularity bit, Gr = 0, limit 在1 Byte(byte粒度)范围内, Gr = 1, limit在4 Kib内(page粒度)
 * SZ: Size bit, SZ = 0, 16位保护模式, SZ = 1, 32位保护模式
 * 
 * 具体参考：https://blog.inlow.online/2019/01/10/GDT/
 */


/**
 * 定义gdt entry，我们指定packed是为了阻止编译器“优化”
 */
struct gdt_entry
{
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t base_middle;
    uint8_t access;
    uint8_t limit_high_flags;
    uint8_t base_high;
} __attribute__((__packed__));

/**
 * 定义指向gdt的结构，也就是GDT Descriptor，包括gdt的大小和gdt的首地址，
 * 同样这里设置为packed
 */
struct gdt_desc
{
    uint16_t size;
    void* offset;
} __attribute__((__packed__));

/**
 * 这里的宏是为了方便设置具体gdt_entry
 */
#define GDT_ENTRY(base, limit, access, flags) \
{ \
    (limit)          & 0xFFFF, \
    (base)           & 0xFFFF, \
    ((base) >> 16)   & 0xFF, \
    (access)         & 0xFF, \
    (((limit) >> 16) & 0x0F) | ((flags) & 0xF0), \
    ((base) >> 24)   & 0xFF \
}

#define GDT_PRESENT ((1 << 7) | (1 << 4))  // 1001 0000
#define GDT_RING0 0
#define GDT_EXECUTABLE (1 << 3)            // 1000 可执行
#define GDT_READ_WRITE (1 << 1)            // 0010 可读写

#define GDT_GRANULARITY_4K (1 << 7)        // 1000 0000 粒度是4Kbyte
#define GDT_PROTECTED_MODE (1 << 6)        // 0100 0000 32位保护模式


#endif /* KERNEL_GDT_H__ */
