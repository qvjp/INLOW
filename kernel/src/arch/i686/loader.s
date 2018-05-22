.extern kernel_main

.global _start

.set MB_ALIGN,    1<<0
.set MB_MEMINFO,  1<<1
.set MB_MAGIC,    0x1BADB002
.set MB_FLAGS,    MB_ALIGN | MB_MEMINFO
.set MB_CHECKSUM, - (MB_MAGIC + MB_FLAGS)

// GRUB启动系统要找的Multiboot header
.section .multiboot
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

// 这个段包含的数据在内核加载时会被初始化为0
.section .bss
    .align 16
    stack_bottom:
        .skip 4096
    stack_top:

// 这个段包含真正要运行的代码
.section .text
    _start:
        // 加载GDT
        mov $gdt_descriptor, %ecx     // 加载GDT的地址
        lgdt (%ecx)                  // 加载GDT
        // CPU通过段寄存器（cs，ds，ss...）找到正确的段，下边将各段寄存器都保存为数据段的地址
        mov $0x10, %cx               // 这里0x10是我们数据段的偏移量, 然后刷新各个寄存器
        mov %cx, %ds                 // 数据段寄存器
        mov %cx, %es                 // 扩展段寄存器
        mov %cx, %fs                 // 标志段寄存器
        mov %cx, %gs                 // 全局段寄存器
        mov %cx, %ss                 // 堆栈段寄存器
        // 其他段寄存器可以直接通过mov来改变，但是代码段寄存器（cs）需要用jmp来加载此段（0x8是代码段偏移量）
        // 这里用ljmp直接跳到下一条指令（1:）并同时设置了cs寄存器
        ljmp $0x8, $1f

    1:
        mov $stack_top, %esp

        call _init
        call kernel_main
        call _fini

        hang:
            cli
            hlt
            jmp hang

.size _start, . - _start
