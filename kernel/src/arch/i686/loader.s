.extern kernel_main

.global _start

/*
 * 声明multiboot header所用到的内容
 * 
 * grub在加载内核之前需要知道内核的基本信息，这里我们给grub的信息是标准的“Multiboot”。
 * 这里定义的“Multiboot header”通过硬编码来实现的。grub就是通过这些信息来确定内核可以
 * 被加载。
 */
.set MB_ALIGN,      1<<0                    /* 在页面边界上对齐加载的模块 */
.set MB_MEMINFO,    1<<1                    /* 提供内存map */
.set MB_MAGIC,      0x1BADB002              /* 这个魔法常量（magic number）让grub可以找到*/ 
.set MB_FLAGS,      MB_ALIGN | MB_MEMINFO   /* 这是Multiboot的“flag” */
.set MB_CHECKSUM,   - (MB_MAGIC + MB_FLAGS) /* 校验上边的常量，提供我们的Multiboot */

/*
 * 这个section包含标记为内核的Multiboot header
 *
 * 这些魔法数字都是grub的multiboot标准定义的，我们按照标准作就可以。
 * grub将在内核最开始的8 KiB内，以32位边界对齐来搜索这个魔法数字。
 */
.section .multiboot
    .align 4
    .long MB_MAGIC
    .long MB_FLAGS
    .long MB_CHECKSUM

/* 
 * 这个段包含的数据在内核加载时会被初始化为0
 *
 * 我们的内核是用C写的，C运行需要栈，所以这里我们申请4096 bytes（4 KiB）的空间给栈。
 */
.section .bss
    .align 16
    stack_bottom:
        .skip 4096
    stack_top:

/*
 * 这个段包含真正要运行的代码
 *
 * linker脚本指定_start为内核的入口，所以grub会在内核加载后跳到这个地方开始执行。
 * 这里没有返回值的原因是grub在加载好内核后就不运行了。
 */
.section .text
    _start:
        /*
         * grub把内核加载到32位保护模式运行，中断被关闭，分页被关闭，处理器的
         * 状态由Multiboot标准定义。内核有对机器的完全控制权。内核只能使用硬件
         * 提供的功能，其他功能都得自己实现，例如不能使用printf函数，除非内核
         * 自己实现了stdio.h的功能。现在没有任何安全限制，没有保护措施，不能
         * Debug，所有东西都要自己提供，但是令人兴奋的是，现在我们拥有至高无
         * 上的权限。
         */

        /*
         * 设置栈，将栈指针寄存器(esp)指向栈顶（x86系统栈是向下增长）。这里必须
         * 用汇编设置好，否则依靠栈的语言（这里特指C）将不能使用。
         */
        mov $stack_top, %esp

        /*
         * 在高级别内核被调用前初始化好处理器状态是很有必要的，这里我们已经做了
         * 最小化的初始化工作，但要注意的是，我们并没有完全初始化，例如GDT没有
         * 被加载，分页没有开启等。
         */

        /*
         * 进入高级别内核。
         * ABI要求在调用指令时栈是32位对齐（然后返回32位的指针）。我们的栈一开
         * 始就是32位对齐，并且到目前为止没有向栈对push任何内容，所以，调用高级
         * 语言编写的函数吧！
         */
        call kernel_main

        /*
         * 如果系统没任何其他要做的事，从高级语言return了，就让系统陷入死循话，来
         * 做这些事：
         * 1. 关闭中断（在eflags中清除中断开启状态）。在我们这里其实在grub中已经关
         * 闭了中断，所以这一步是多余的，这里设置是为了将来开启了中断忘了关闭：）
         * 2. 等待下一个中断（hlt）到达，因为我们已经关闭了中断，所以这将锁定计算机。
         * 3. 如果因为某些奇怪的原因在上边的指令后还没有关闭计算机，跳到1再执行一次。
         */
        hang:
            cli
            hlt
            jmp hang

/*
 * 用_start保存当前位置减去它开始的位置的大小，后边用的着。
 */
.size _start, . - _start
