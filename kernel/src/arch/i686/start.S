.section multiboot
.align 4
.long 0x1BADB002
.long 0x3
.long -(0x1BADB002 + 0x3)

.section .text
.global _start
.type _start, @function
_start:
	# load the GDT
	mov $gdt_descriptor, %ecx
	lgdt (%ecx)
	mov $0x10, %cx
	mov %cx, %ds
	mov %cx, %es
	mov %cx, %fs
	mov %cx, %gs
	mov %cx, %ss
	ljmp $0x8, $1f

1:	mov $kernel_stack, %esp

	push $0
	push $0
	mov %esp, %ebp

	push %ebx
	push %eax

	call _init
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
