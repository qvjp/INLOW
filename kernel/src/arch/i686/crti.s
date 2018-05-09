/*
 System V ABI用5个特殊的文件一起控制程序的初始化，即crt0.o, crti.o
 crtbegin.o, crtend.o, crtn.o, 这些文件共同实现了两个函数_init, _fini
 正如字面意思，这两个函数分别运行全局构造函数和其他初始化任务，_fini
 运行全局析构函数和其他终止任务

 GCC在编译时会自动将
 i686-elf-gcc foo.o bar.o -o program重写成如下格式
 i686-elf-ld crt0.o crti.o crtbegin.o foo.o bar.o crtend.o crtn.o
 其中crtbegin.o和crtend.o交叉编译器会提供
 本文件即是提供crti.o
*/

.section .init
.global _init

_init:
    /*
     下边两行是保存ebp(基址寄存器)和调用前的esp(栈顶地址)，即保存整个栈的内容
     方便调用后的恢复
    */
    push %ebp
    mov %esp, %ebp
    /*GCC将把crtbegin.o的.init section 放到这里*/

.section .fini
.global _fini
_fini:
    /*下边两行也是保存ebp和esp，这是为了保存整个栈的内容*/
    push %ebp
    mov %esp, %ebp
    /*GCC将把crtbegin.o的.fini section 放到这里*/
