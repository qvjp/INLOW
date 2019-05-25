.section .text
.global _start
.type _start, @function
_start:
    push $0
    push $0
    mov %esp, %ebp

    sub $12, %esp
    push %ecx
    push %ebx
    push %eax

    call _init
    call main

    add $4, %esp
    push %eax
    call exit

.size _start, . - _start
