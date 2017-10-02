.section multiboot
.align 4
.long 0x1BADB002
.long 0x3
.long -(0x1BADB002 + 0x3)

.section .text
.global _start
.type _start, @function
_start:
	mov $kernel_stack, %esp

	push $0
	push $0
	mov %esp, %ebp

	push %ebx
	push %eax

	call kernel_main

_halt:
	cli
	hlt
	jmp _halt
.size _start, . - _start

.section .bss
.align 4096
.skip 4096

kernel_stack:
