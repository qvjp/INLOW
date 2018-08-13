.extern kernel_main

.global _start

.set MB_ALIGN,    1<<0
.set MB_MEMINFO,  1<<1
.set MB_MAGIC,    0x1BADB002
.set MB_FLAGS,    MB_ALIGN | MB_MEMINFO
.set MB_CHECKSUM, - (MB_MAGIC + MB_FLAGS)

/* GRUB启动系统要找的Multiboot header */
.section .multiboot
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

/* 下边两个section是启用分页之前，以地址0x100000运行的代码 */
/* 存放页目录和页表 */
.section bootstrap_bss, "aw", @nobits
    .align 4096
    pageDirectory:
        .skip 4096
    bootstrapPageTable:
        .skip 4096
    kernelPageTable:
        .skip 4096
    physicalMemoryPageTable:
        .skip 4096

/* 设置页，跳到高地址内核 */
.section bootstrap_text, "ax"
    _start:
        # 添加页表到页目录
        movl $(bootstrapPageTable + 0x3), pageDirectory
        movl $(kernelPageTable + 0x3), pageDirectory + 0xC00
        movl $(physicalMemoryPageTable + 0x3), pageDirectory + 0xFF8

        # 递归映射(recursively mapping)页目录到0xFFC00000
        movl $(pageDirectory + 0x3), pageDirectory + 0xFFC

        # 恒等映射(identity mapping)bootstrap section
        mov $numBootstrapPages, %ecx
        mov $(bootstrapPageTable + 256 * 4), %edi
        mov $(bootstrapBegin + 3), %edx
    1:
        mov %edx, (%edi)
        # bootstrapPageTable中入口大小为4 bytes
        add $4, %edi
        # 页大小为4096 bytes
        add $4096, %edx
        # 循环直到最后一个入口
        loop 1b

        # 映射内核
        mov $numKernelPages, %ecx
        add $(kernelPageTable - bootstrapPageTable), %edi
        mov $(kernelPhysicalBegin + 3), %edx
    1:
        mov %edx, (%edi)
        # kernelPageTable中入口大小为4 bytes
        add $4, %edi
        # 页大小为4096 bytes
        add $4096, %edx
        # 循环直到最后一个入口
        loop 1b

        # 映射视频内存到0xC0000000
        movl $0xB8003, kernelPageTable

        /* 开启分页 */
        mov $pageDirectory, %ecx
        mov %ecx, %cr3

        mov %cr0, %ecx       /* 读取当前cr0 */
        or $0x80000000, %ecx /* 设置PG */
        mov %ecx, %cr0       /* 更新cr0 */

        # 跳转到高地址空间
        jmp _start_high
.size _start, . - _start

/* 下边两个section是启用分页之后代码所在位置 */
/* bss包含的数据在内核加载时会被初始化为0 */
.section .bss
    .align 16
    stack_bottom:
        .skip 4096
    stack_top:

/* text是可执行代码所放位置 */
.section .text
    _start_high:
        /* 加载GDT */
        mov $gdt_descriptor, %ecx     /* 加载GDT的地址 */
        lgdt (%ecx)                  /* 加载GDT */
        /* CPU通过段寄存器（cs，ds，ss...）找到正确的段，下边将各段寄存器都保存为数据段的地址 */
        mov $0x10, %cx               /* 这里0x10是我们数据段的偏移量, 然后刷新各个寄存器 */
        mov %cx, %ds                 /* 数据段寄存器 */
        mov %cx, %es                 /* 扩展段寄存器 */
        mov %cx, %fs                 /* 标志段寄存器 */
        mov %cx, %gs                 /* 全局段寄存器 */
        mov %cx, %ss                 /* 堆栈段寄存器 */
        /* 其他段寄存器可以直接通过mov来改变，但是代码段寄存器（cs）需要用jmp来加载此段（0x8是代码段偏移量） */
        /* 这里用ljmp直接跳到下一条指令（1:）并同时设置了cs寄存器 */
        ljmp $0x8, $1f

        /* 加载IDT */
    1:
        mov $idt_descriptor, %ecx    /* 加载IDT地址 */
        lidt (%ecx)                  /* 加载IDT */
        
        mov $stack_top, %esp

        push $0
        push $0
        mov %esp, %ebp

        push %ebx /* multiboot */
        push %eax /* magic */

        call _init
        call kernel_main
        call _fini

        hang:
            cli
            hlt
            jmp hang

.size _start_high, . - _start_high
