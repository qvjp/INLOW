.section .text
.global syscallHandler
.type syscallHandler, @function
syscallHandler:
	cld

	mov %esp, %ebp
	and $(~0xFF), %esp
	sub $12, %esp
	push %edi

	push %esi
	push %edx
	push %ecx
	push %ebx

	sub $12, %esp
	push %eax

	mov $0x10, %cx
	mov %cx, %ds
	mov %cx, %es

	call getSyscallHandler
	add $16, %esp
	movl $0, errno

	call *%eax

	mov $0x23, %cx
	mov %cx, %ds
	mov %cx, %es

	mov errno, %ecx
	mov %ebp, %esp
	iret

.size syscallHandler, . - syscallHandler
