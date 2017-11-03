.section .text
.global __syscall
.type __syscall, @function
__syscall:
	push %ebp
	mov %esp, %ebp
	
	# Preserve caller saved registers
	push %edi
	push %esi
	push %ebx
	sub $12, %esp

	# Put the arguments into registers
	mov 8(%ebp), %ebx
	mov 12(%ebp), %ecx
	mov 16(%ebp), %edx
	mov 20(%ebp), %esi
	mov 24(%ebp), %edi

	int $0x30

	test %ecx, %ecx
	jz 1f
	mov %ecx, errno

1:  add $12, %esp
	pop %ebx
	pop %esi
	pop %edi
	pop %ebp
	ret

.size __syscall, . - __syscall
