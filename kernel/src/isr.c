#include <inlow/kernel/idt.h> /* idt_set_gate() */
#include <inlow/kernel/isr.h>
#include <inlow/kernel/print.h> /* printf() */



void isrs_install()
{
    /**
     * 挨个设置IDT中的ISR
    */
    idt_set_gate(0, (uint32_t)isr_0, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(1, (uint32_t)isr_1, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(2, (uint32_t)isr_2, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(3, (uint32_t)isr_3, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(4, (uint32_t)isr_4, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(5, (uint32_t)isr_5, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(6, (uint32_t)isr_6, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(7, (uint32_t)isr_7, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(8, (uint32_t)isr_8, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(9, (uint32_t)isr_9, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(10, (uint32_t)isr_10, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(11, (uint32_t)isr_11, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(12, (uint32_t)isr_12, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(13, (uint32_t)isr_13, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(14, (uint32_t)isr_14, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(15, (uint32_t)isr_15, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(16, (uint32_t)isr_16, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(17, (uint32_t)isr_17, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(18, (uint32_t)isr_18, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(19, (uint32_t)isr_19, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(20, (uint32_t)isr_20, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(21, (uint32_t)isr_21, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(22, (uint32_t)isr_22, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(23, (uint32_t)isr_23, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(24, (uint32_t)isr_24, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(25, (uint32_t)isr_25, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(26, (uint32_t)isr_26, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(27, (uint32_t)isr_27, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(28, (uint32_t)isr_28, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(29, (uint32_t)isr_29, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(30, (uint32_t)isr_30, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
    idt_set_gate(31, (uint32_t)isr_31, 0x8, IDT_INTERRUPT_GATE | IDT_RING0 | IDT_PRESENT);
}

/*
 * 每个ISR对应的错误信息
*/
char* exception_messages[] =
{
    "Devide-by-zero",
    "Debug",
    "Non-maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "x87 Floating-Point",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point",
    "Virtualization",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void exception_handler(struct regs* r)
{
    /**
     * 我们所有的ISR都将通过这个方法进入具体的处理程序
     * 现在只是简单的打印异常信息并通过一个死循环停止系统运行
    */
    if (r->int_no < 32)
    {
        printf("eax: 0x%x, ebx: 0x%x, ecx: 0x%x, edx: 0x%x\n", r->eax, r->ebx, r->ecx, r->edx);
        printf("edi: 0x%x, esi: 0x%x, ebp: 0x%x, esp: 0x%x\n", r->edi, r->esi, r->ebp, r->esp);
        printf("cs: 0x%x, eip: 0x%x, eflags: 0x%x, ss: 0x%x\n", r->cs, r->eip, r->eflags, r->ss);
        printf("%s", exception_messages[r->int_no]);
        printf(" Exception. System Halted!\n");
        for (;;);
    }
}
