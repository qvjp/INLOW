.section .text
.global _start
.type _start, @function
_start:

	# Create a stack frame
	push $0
	push $0
	mov %esp, %ebp

	sub $12, %esp
	push %ecx # envp
	push %ebx # argv
	push %eax # argc

	# Call global consrtuctors
	call _init
	
	call main

	add $4, %esp
	push %eax
	call exit

.size _start, . - _start
