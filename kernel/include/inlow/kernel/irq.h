#ifndef KERNEL_IRQ_H__
#define KERNEL_IRQ_H__

#include <inlow/kernel/isr.h> /* struct regs */

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1
#define PIC_EOI 0x20 // End of Interrupt 

// 声明每个IRQ专门的处理程序
extern void irq_0(void);    // Programmable Interrupt Timer Interrupt
extern void irq_1(void);    // Keyboard Interrupt
extern void irq_2(void);    // Cascade (used internally by the two PICs. never raised)
extern void irq_3(void);    // COM2 (if enabled)
extern void irq_4(void);    // COM1 (if enabled)
extern void irq_5(void);    // LPT2 (if enabled)
extern void irq_6(void);    // Floppy Disk
extern void irq_7(void);    // LPT1 / Unreliable "spurious" interrupt (usually)
extern void irq_8(void);    // CMOS real-time clock (if enabled)
extern void irq_9(void);    // Free for peripherals / legacy SCSI / NIC
extern void irq_10(void);   // Free for peripherals / SCSI / NIC
extern void irq_11(void);   // Free for peripherals / SCSI / NIC
extern void irq_12(void);   // PS2 Mouse
extern void irq_13(void);   // FPU / Coprocessor / Inter-processor
extern void irq_14(void);   // Primary ATA Hard Disk
extern void irq_15(void);   // Secondary ATA Hard Disk

/**
 * 将上边声明的处理程序设置到IDT当中
 */
extern void irq_install();

/**
 * 所有的IRQ发生后都指向这个函数，
 */
extern void irq_handler(struct regs *r);

/**
 * 设置自定义的IRQ处理程序
 */
extern void irq_install_handler(int irq, void (*handler)(struct regs *r));

/**
 * 清除所给IRQ的处理程序
 */
extern void irq_uninstall_handler(int irq);

/**
 * IRQ的0～7被映射到IDT的8～15，IRQ的8～15被映射到70h～78h，然而IDT的
 * 8号是Double Fault Exception，这就导致每次IRQ0发生时，你都会得到一个
 * Double Fault Exception，其实这个异常并没有发生。
 * 所以，我们需要重新映射，将IRQ0～15映射到IDT的32～47
 */
extern void irq_remap(void);

/**
 * 启动中断
 */
extern void irq_enable(void);

#endif /* KERNEL_IRQ_H__ */
