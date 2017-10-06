.global _start
_start:
	mov $0, %eax
	mov $42, %ebx
	int $0x30
