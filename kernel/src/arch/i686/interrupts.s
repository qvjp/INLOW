.section .text

.macro isr no
.global isr_\no
isr_\no:
	push $0 
	push $\no
	jmp commonHandler
.endm

.macro isr_error_code no
.global isr_\no
isr_\no:
	push $\no
	jmp commonHandler
.endm

commonHandler:
	cld

	push %ebp
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	push %eax

	# Switch to kernel data segment
	mov $0x10, %ax
	mov %ax, %ds
	mov %ax, %es

	mov %esp, %eax
	and $(~0xFF), %esp
	sub $12, %esp
	push %eax

	call handleInterrupt
	mov %eax, %esp

	# Check whether signals are pending
	mov signalPending, %ebx
	test %ebx, %ebx
	jz 1f

	# Don't handle signals when returning to kernespace
	mov 40(%esp), %ebx # cs
	cmp $0x8, %ebx
	je 1f

	and $(~0xFF), %esp # Align the stack
	sub $12, %esp
	push %eax
	call handleSignal
	mov %eax, %esp

	# Switch back to user data segment
1:	mov $0x23, %ax
	mov %ax, %ds
	mov %ax, %es

	pop %eax
	pop %ebx
	pop %ecx
	pop %edx
	pop %esi
	pop %edi
	pop %ebp

	add $8, %esp

	iret

# CPU Exceptions
isr 0				# Devide-by-zero
isr 1				# Debug
isr 2				# Non-maskable Interrupt
isr 3				# Breakpoint
isr 4				# Overflow
isr 5				# Bound Range Exceeded
isr 6				# Invalid Opcode
isr 7				# Device Not Available
isr_error_code 8	# Double Fault
isr 9				# Coprocessor Segment Overrun
isr_error_code 10	# Invalid TSS 
isr_error_code 11	# Segment Not Present 
isr_error_code 12	# Stack-Segment Fault 
isr_error_code 13 	# General Protection Fault
isr_error_code 14 	# Page Fault 
isr 15				# Reserved 
isr 16				# x87 Floating-Point Exception 
isr_error_code 17	# Alignment Check 
isr 18				# Machine Check 
isr 19				# SIMD Floating-Point Exception 
isr 20				# Virtualization Exception

# IRQs
isr 32
isr 33
isr 34
isr 35
isr 36
isr 37
isr 38
isr 39
isr 40
isr 41
isr 42
isr 43
isr 44
isr 45
isr 46
isr 47

# isr 48 # Syscall
isr 49 # Schedule
