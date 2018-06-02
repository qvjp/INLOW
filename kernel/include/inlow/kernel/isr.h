#ifndef KERNEL_ISR_H__
#define KERNEL_ISR_H__

#include <stdint.h> /* uint32_t */

#define IDT_INTERRUPT_GATE 0xE
#define IDT_RING0 0
#define IDT_PRESENT (1 << 7)

// CPU Exceptions 处理CPU异常的方法
extern void isr_0(void);     // Devide-by-zero Exception
extern void isr_1(void);     // Debug Exception
extern void isr_2(void);     // Non-maskable Interrupt Exception
extern void isr_3(void);     // Breakpoint Exception
extern void isr_4(void);     // Into Detected Overflow Exception
extern void isr_5(void);     // Bound Range Exceeded Exception
extern void isr_6(void);     // Invalid Opcode Exception
extern void isr_7(void);     // No Coprocessor Exception
extern void isr_8(void);     // Double Fault Exception
extern void isr_9(void);     // Coprocessor Segment Overrun Exception
extern void isr_10(void);    // Invalid TSS Exception
extern void isr_11(void);    // Segment Not Present Exception
extern void isr_12(void);    // Stack-Segment Fault Exception
extern void isr_13(void);    // General Protection Fault Exception
extern void isr_14(void);    // Page Fault Exception
extern void isr_15(void);    // Unknown Interrupt Exception
extern void isr_16(void);    // x87 Floating-Point Exception
extern void isr_17(void);    // Alignment Check Exception
extern void isr_18(void);    // Machine Check
extern void isr_19(void);    // SIMD Floating-Point Exception
extern void isr_20(void);    // Virtualization Exception
extern void isr_21(void);    // Reserved Exception
extern void isr_22(void);    // Reserved Exception
extern void isr_23(void);    // Reserved Exception
extern void isr_24(void);    // Reserved Exception
extern void isr_25(void);    // Reserved Exception
extern void isr_26(void);    // Reserved Exception
extern void isr_27(void);    // Reserved Exception
extern void isr_28(void);    // Reserved Exception
extern void isr_29(void);    // Reserved Exception
extern void isr_30(void);    // Reserved Exception
extern void isr_31(void);    // Reserved Exception

/**
 通用寄存器
 uint32_t eax;   Accumulator Register 累加器
 uint32_t ebx;   Base Register 基地址寄存器
 uint32_t ecx;   Count Register 计数寄存器
 uint32_t edx;   Data Register 数据寄存器

 变址寄存器 主要存放存储单元在段内的偏移量
 uint32_t esi;
 uint32_t edi;

 指针寄存器 主要用于存放堆栈内存储单元的偏移量
 uint32_t ebp;   Base Pointer Register 基指针寄存器
 uint32_t esp;   Stack Pointer Register 栈指针寄存器

 段寄存器
 uint32_t cs;    Code Segment Register 代码段寄存器
 uint32_t ds;    Data Segment Register 数据段寄存器
 uint32_t es;    Extra Segment Register 附加段寄存器
 uint32_t fs;
 uint32_t gs;
 uint32_t ss;    Stack Segment Register 堆栈段寄存器

 指令指针寄存器
 uint32_t eip;   Instruction Pointer 存放下一条指令在代码段的偏移量
 */

/**
 * 这个结构定义ISR运行时栈的样子
 */
struct regs
{
    uint32_t gs, fs, es, ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* "pasha" 将数据压如这几个寄存器 */
    uint32_t int_no, err_code;    /* 我们"push $\no"就是将错误吗push到int_no中 */
    uint32_t eip, cs, eflags, useresp, ss;  
};

/**
 * 将上边声明的处理程序设置到IDT当中
 */
extern void isrs_install();

/**
 * 所有的ISR发生后都指向这个函数，
 */
extern void exception_handler(struct regs* r);

#endif /* KERNEL_ISR_H__ */
