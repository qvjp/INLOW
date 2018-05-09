/*
 bootloader在系统镜像中找下边指出的入口
*/
ENTRY(_start);

/*
 指定各段在最终镜像中的位置
*/
SECTIONS {
    /*
     从1M处开始放我们的各种段，因为1M以内是x86保留的，我们不能用
    */
    . = 1M;

    /*
     把所有段都设置为4K的倍数，将来增加‘页’的时候会有用
    */

    /*
     可执行代码段
     最开始放multiboot，因为如果远了bootloader会找不到
    */
    .text BLOCK(4K) : ALIGN(4K)
    {
        *(.multiboot)
        *(.text)
    }

    /*
     只读数据
    */
    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    /*
     可读可写数据（将被初始化）
    */
    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    /*
     可读可写数据（未初始化）和栈
    */
    .bss BLOCK(4K) : ALIGN(4K)
    {
        *(COMMON)
        *(.bss)
    }
}
