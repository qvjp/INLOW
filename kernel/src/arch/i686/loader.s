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
        mov $stack_top, %esp

        call kernel_main

        hang:
            cli
            hlt
            jmp hang

.size _start, . - _start
