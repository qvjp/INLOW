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
	mov %ebp, %esp

	# Check whether signals are pending
	mov signalPending, %ecx
	test %ecx, %ecx
	jnz 2f

	mov $0x23, %cx
	mov %cx, %ds
	mov %cx, %es

	mov errno, %ecx

1:	iret

	# Fake an InterruptContext so that we can call handleSignal
2:	sub $8, %esp
	push %ebp
	push %edi
	push %esi
	push %edx
	pushl errno
	push %ebx
	push %eax

	mov %esp, %eax
	and $(~0xFF), %esp # Align the stack
	sub $12, %esp
	push %eax

	call handleSignal
	mov %eax, %esp

	pop %eax
	pop %ebx
	pop %ecx
	pop %edx
	pop %esi
	pop %edi
	pop %ebp
	add $8, %esp

	jmp 1b


.size syscallHandler, . - syscallHandler
