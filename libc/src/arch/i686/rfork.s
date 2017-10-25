.global rfork
.type rfork, @function
rfork:
	push %ebp
	mov %esp, %ebp
	sub $12, %esp
	
	# Push struct regfork
	push %ebp
	push %edi
	push %esi
	push %edx
	push %ecx
	push %ebx
	# In the child process this return 0
	push $0
	push $afterFork
	# The value for %esp does not matter because leave will modify %esp anyway
	push $0

	push %esp
	push 8(%ebp)

	call regfork
afterFork:
	leave
	ret
.size rfork, . - rfork
