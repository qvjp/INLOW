#ifndef KERNEL_IDT_H__
#define KERNEL_IDT_H__

#include <stdint.h> /* uint16_t uint8_t uint32_t */

/*
                              * GDT ENTRT *
|-------------------------------------------------------------------------|
|                     |        |        |                |                |
|     Offset2         | Access |  Zero  |    Selector    |     Offset1    |
|      16:31          |        | 16:23  |      0:15      |      0:15      |
|                     |        |        |                |                |
|-------------------------------------------------------------------------|
|63     56|55 52|51 48|47    40|39    32|31            16|15             0| 

Offset: 32 bit, 被分为两部分。存放ISR入口地址
Selector: 16 bit, GDT的代码段selector
Zero: 8 bit, 没有被使用，设置为0
Access: 8 bit, 类型和属性

            * Access Byte *
|-------------------------------------|
|    |       |    |                   |
| Pr |  DPL  | S  |       TYPE        |
|    |       |    |                   |
|-------------------------------------|
|    7       5    4    3    2    1   0|

Pr: Present, 段没有使用设为0, 使用设为1
DPL: Descriptor Privilege Level, 指定调用此描述符所属最小特权级
S: Storage Segment, 设置为0
TYPE: 设为1110时是interrupt gate
*/

/**
 * 定义GDT入口，我们指定packed是为了阻止编译器“优化”
 */
struct idt_entry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t unused;
    uint8_t flags;
    uint16_t offset_high;
} __attribute__((__packed__));

/**
 * 定义指向idt的结构，包括idt的大小和idt的首地址，和GDT时一样
 * 同样这里设置为packed
 */
struct idt_ptr
{
    uint16_t size;
    void* idtp;
} __attribute__((__packed__));

/**
 * 设置IDT中每一个入口
 * num: 中断号
 * offset: 中断服务例程入口偏移
 * selector: 中断服务例程所在段
 * flags: 权限
 */
extern void idt_set_gate(uint8_t num, uint32_t offset, uint16_t selector, uint8_t flags);

#endif /* KERNEL_IDT_H__ */
