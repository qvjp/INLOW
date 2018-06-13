/*
 bootloader在系统镜像中找下边指出的入口
*/
ENTRY(_start);

/* 内核虚拟空间开始位置 */
KERNEL_VIRTUAL = 0xC0000000;

/*
 指定各段在最终镜像中的位置
*/
SECTIONS {
    /*
     从1M处开始放我们的各种段，因为1M以内是x86保留的，我们不能用
    */
    . = 1M;
    bootstrapBegin = .;

    bootstrap_text : {
        /* 最开始放multiboot，因为如果远了bootloader会找不到 */
        *(.multiboot)
        *(bootstrap_text)
    }

    bootstrap_bss ALIGN(4K) : {
        *(bootstrap_bss)
    }
    bootstrapEnd = ALIGN(4K);
    numBootstrapPages = (bootstrapEnd - bootstrapBegin) / 4K;

    /* 内核放在0xC0100000处 */
    . += KERNEL_VIRTUAL;
    kernelVirtualBegin = ALIGN(4K);
    kernelPhysicalBegin = kernelVirtualBegin - KERNEL_VIRTUAL;

    /*
     可执行代码段
    */
    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VIRTUAL)
    {
        *(.text)
    }

    /*
     只读数据
    */
    .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VIRTUAL)
    {
        *(.rodata)
    }

    /*
     可读可写数据（将被初始化）
    */
    .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VIRTUAL)
    {
        *(.data)
    }

    /*
     可读可写数据（未初始化）和栈
    */
    .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VIRTUAL)
    {
        *(COMMON)
        *(.bss)
    }

    kernelVirtualEnd = ALIGN(4K);
    kernelPhysicalEnd = kernelVirtualEnd - KERNEL_VIRTUAL;
    numKernelPages = (kernelVirtualEnd - kernelVirtualBegin) / 4K;
}
