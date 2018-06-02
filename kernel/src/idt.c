#include <inlow/kernel/idt.h>

/**
 * 中断(Interrupt)是指一个来自设备的信号，比如键盘，键盘被按下，CPU就要停下当前正在干的事情，来响应
 * 键盘，这类中断被称为中断请求(IRQ)也叫硬中断。或是来自CPU内部的信号，比如除零，页错误等等，这类中断
 * 又叫异常(Exception)，第三种是大家熟悉的软中断(Software Interrupt)，比如系统调用。CPU如何知道
 * 中断来了之后要怎样处理，这里就需要用到中断描述表(IDT)了，IDT保存了每个中断对应的处理方法，x86的CPU
 * 有256个中断向量，被编号为0～255。
 */

/**
 * 定义256个idt入口，应为CPU上有256个中断向量，虽然说后边我们只使用前32个。
 */
struct idt_entry idt[256];

/**
 * 设置IDT中每一个入口具体内容
 * 具体调用此函数在后边要写的isr irq中
 */
void idt_set_gate(uint8_t num, uint32_t offset, uint16_t selector, uint8_t flags)
{
    idt[num].offset_low = (offset & 0xFFFF);
    idt[num].selector = selector;
    idt[num].unused = 0;
    idt[num].flags = flags;
    idt[num].offset_high = (offset >> 16) & 0xFFFF;
}

/**
 * 汇编代码lidt将会加载这里
 */
struct idt_ptr idt_descriptor = 
{
    sizeof(idt) - 1,
    idt
};
