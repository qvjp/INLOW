.section .text

/* 处理没有错误码的ISR */
.macro isr no
.global isr_\no
isr_\no:
    push $0   /* 对于没有错误码的异常，push一个0是为统一栈帧 */
    push $\no
    jmp isr_commonHandler
.endm

/* 处理有错误码的ISR */
.macro isr_error_code no
.global isr_\no
isr_\no:
    /* 有错误码的isr就不需要手动push了，已经自动push进错误码了 */
    push $\no
    jmp isr_commonHandler
.endm

/* 处理IRQ */
.macro irq no
.global irq_\no
irq_\no:
    push $0
    /* 这里给IRQ编号加32是为了让IRQ在IDT中从32开始 */
    mov $\no, %eax
    add $32, %eax
    push %eax
    jmp irq_commonHandler
.endm

/* 当ISR发生时调用
   保存当前运行状态，设置内核模式段，调用C语言的错误处理程序，最后回到之前的栈桢 */
isr_commonHandler:
    pusha          /* push edi, esi, ebp, esp, ebx, edx, ecx, eax */
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax /* 0x10是内核数据段的偏移 */
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    call exception_handler

    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa         /* pop edi, esi, ebp... */

    add $8, %esp /* 从栈中移除错误码和ISR号 */
    iret         /* pop cs, eip, eflags, ss, esp */

/* 当IRQ发生时调用 除了调用的C语言函数一同之外和ISQ处理方式一样
   保存当前运行状态，设置内核模式段，调用C语言的错误处理程序，最后回到之前的栈桢 */
irq_commonHandler:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %gs
    mov %esp, %eax
    push %eax
    call irq_handler

    pop %eax
    pop %gs
    pop %fs
    pop %es
    pop %ds
    popa

    add $8, %esp
    iret

/* Interrupt        Service Routines                          Error Code */
isr 0               /* Devide-by-zero Exception               No */
isr 1               /* Debug Exception                        No */
isr 2               /* Non-maskable Interrupt Exception       No */
isr 3               /* Breakpoint Exception                   No */
isr 4               /* Into Detected Overflow Exception       No */
isr 5               /* Bound Range Exceeded Exception         No */
isr 6               /* Invalid Opcode Exception               No */
isr 7               /* No Coprocessor Exception               No */
isr_error_code 8    /* Double Fault Exception                 Yes */
isr 9               /* Coprocessor Segment Overrun Exception  No */
isr_error_code 10   /* Invalid TSS Exception                  Yes */
isr_error_code 11   /* Segment Not Present Exception          Yes */
isr_error_code 12   /* Stack-Segment Fault Exception          Yes */
isr_error_code 13   /* General Protection Fault Exception     Yes */
isr_error_code 14   /* Page Fault Exception                   Yes */
isr 15              /* Unknown Interrupt Exception            No */
isr 16              /* x87 Floating-Point Exception           No */
isr 17              /* Alignment Check Exception              No */
isr 18              /* Machine Check                          No */
isr 19              /* SIMD Floating-Point Exception          No */
isr 20              /* Virtualization Exception               No */
isr 21              /* Reserved Exception */
isr 22              /* Reserved Exception */
isr 23              /* Reserved Exception */
isr 24              /* Reserved Exception */
isr 25              /* Reserved Exception */
isr 26              /* Reserved Exception */
isr 27              /* Reserved Exception */
isr 28              /* Reserved Exception */
isr 29              /* Reserved Exception */
isr 30              /* Reserved Exception */
isr 31              /* Reserved Exception */

/* Interrupt Requests */
irq 0               /* Programmable Interrupt Timer Interrupt */
irq 1               /* Keyboard Interrupt */
irq 2               /* Cascade (used internally by the two PICs. never raised) */
irq 3               /* COM2 (if enabled) */
irq 4               /* COM1 (if enabled) */
irq 5               /* LPT2 (if enabled) */
irq 6               /* Floppy Disk */
irq 7               /* LPT1 / Unreliable "spurious" interrupt (usually) */
irq 8               /* CMOS real-time clock (if enabled) */
irq 9               /* Free for peripherals / legacy SCSI / NIC */
irq 10              /* Free for peripherals / SCSI / NIC */
irq 11              /* Free for peripherals / SCSI / NIC */
irq 12              /* PS2 Mouse */
irq 13              /* FPU / Coprocessor / Inter-processor */
irq 14              /* Primary ATA Hard Disk */
irq 15              /* Secondary ATA Hard Disk */
