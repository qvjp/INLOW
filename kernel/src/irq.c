#include <inlow/kernel/idt.h> /* idt_set_gate() */
#include <inlow/kernel/irq.h>
#include <inlow/kernel/port.h> /* outportb() */
#include<inlow/kernel/print.h> /* printf() */

/*
* 这个数组是函数指针数组，用它来放自定义的IRQ处理程序
*/
void* irq_routines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void irq_install_handler(int irq, void (*handler)(struct regs *r))
{
    irq_routines[irq] = handler;
}

void irq_uninstall_handler(int irq)
{
    irq_routines[irq] = 0;
}


void irq_remap(void)
{
    // 开始初始化序列
    outportb(PIC1_COMMAND, 0x11);
    outportb(PIC2_COMMAND, 0x11);

    // 主PIC向量偏移0x20(也就是主PIC重新映射后是从IDT的32-39)，从PIC向量偏移0x28(从PIC40-47)
    outportb(PIC1_DATA, 0x20);
    outportb(PIC2_DATA, 0x28);

    // 告诉主PIC在IRQ2(0000 0100)有从PIC
    outportb(PIC1_DATA, 0x04);
    // 告诉从PIC它的层叠号吗
    outportb(PIC2_DATA, 0x02);

    outportb(PIC1_DATA, 0x01);
    outportb(PIC2_DATA, 0x01);

    outportb(PIC1_DATA, 0x0);
    outportb(PIC2_DATA, 0x0);
}

void irq_install()
{
    /**
    * 先重新映射了IRQ在IDT中的位置，再在IDT正确的位置放适当的IRQ处理程序
    */
    irq_remap();

    idt_set_gate(32, (unsigned)irq_0, 0x08, 0x8E);
    idt_set_gate(33, (unsigned)irq_1, 0x08, 0x8E);
    idt_set_gate(34, (unsigned)irq_2, 0x08, 0x8E);
    idt_set_gate(35, (unsigned)irq_3, 0x08, 0x8E);
    idt_set_gate(36, (unsigned)irq_4, 0x08, 0x8E);
    idt_set_gate(37, (unsigned)irq_5, 0x08, 0x8E);
    idt_set_gate(38, (unsigned)irq_6, 0x08, 0x8E);
    idt_set_gate(39, (unsigned)irq_7, 0x08, 0x8E);
    idt_set_gate(40, (unsigned)irq_8, 0x08, 0x8E);
    idt_set_gate(41, (unsigned)irq_9, 0x08, 0x8E);
    idt_set_gate(42, (unsigned)irq_10, 0x08, 0x8E);
    idt_set_gate(43, (unsigned)irq_11, 0x08, 0x8E);
    idt_set_gate(44, (unsigned)irq_12, 0x08, 0x8E);
    idt_set_gate(45, (unsigned)irq_13, 0x08, 0x8E);
    idt_set_gate(46, (unsigned)irq_14, 0x08, 0x8E);
    idt_set_gate(47, (unsigned)irq_15, 0x08, 0x8E);
}


void irq_handler(struct regs *r)
{
    /*
     * 定义一个空函数指针用来放具体的IRQ处理程序
    */
    void (*handler)(struct regs *r);

    if (r->int_no <= 47 || r->int_no >= 32)
    {
        if (r->int_no != 32)
            printf("IRQ %d occurred!\n", r->int_no - 32);
        handler = irq_routines[r->int_no - 32];
        if (handler)
        {
            handler(r);
        }
        /*
         * 中断处理结束后，要发送EOI(End Of Interrupt)给PIC的命令端口
         * 如果IDT入口号大于40(IRQ8-15)，也就是说这个IRQ来自从PIC，我们需要给从PIC的命令端口发送一个EOI 
         * 而如果IRQ来自主PIC，我们只需要向主PIC发送EOI即可
        */
        if (r->int_no >= 40)
        {
            outportb(PIC2_COMMAND, PIC_EOI);
        }
        outportb(PIC1_COMMAND, PIC_EOI);
    }
    else
    {
        printf("Unknow interrupt %u!\n", r->int_no);
    }
}

void irq_enable(void)
{
    /**
    * 开启中断
    */
    __asm__ __volatile__ ("sti");
}
