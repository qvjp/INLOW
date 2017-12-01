#define CR0_WRITE_PROTECT (1 << 16)
#define CR0_PAGING_ENABLE (1 << 31)
#define PAGE_READONLY 0x1
#define PAGE_WRITE 0x3

.section multiboot
.align 4
.long 0x1BADB002
.long 0x3
.long -(0x1BADB002 + 0x3)

.section bootstrap_text, "ax"
.global _start
.type _start, @function
_start:
	cld

	# Add the page tables to the page directory
    movl $(pageTableBootstrap + 0x3), kernelPageDirectory
    movl $(pageTableKernel + 0x3), kernelPageDirectory + 0xC00
	movl $(pageTablePhysicalMemory + 0x3), kernelPageDirectory + 0xFF8

	# Recursively map the page directory at 0xFFC00000
	movl $(kernelPageDirectory + 0x3), kernelPageDirectory + 0xFFC

    # Identity-map the bootstrap section
    mov $numBootstrapPages, %ecx
    mov $(pageTableBootstrap + 256 * 4), %edi
    mov $(bootstrapBegin + 0x3), %edx

1:  mov %edx, (%edi)
    add $4, %edi
    add $0x1000, %edx
    loop 1b

    # Map readonly part of the kernel
    mov $numReadOnlyPages, %ecx
    add $(pageTableKernel - pageTableBootstrap), %edi
    mov $(kernelPhysicalBegin + 0x1), %edx

1:  mov %edx, (%edi)
	add $4, %edi
	add $0x1000, %edx
	loop 1b

	# Map wirteable part
	mov $numWritablePages, %ecx
	or $0x3, %edx

1:  mov %edx, (%edi)
    add $4, %edi
    add $0x1000, %edx
    loop 1b

    # Map video memory at 0xC0000000.
    movl $0xB8003, pageTableKernel

    # Enable paging
    mov $kernelPageDirectory, %ecx
    mov %ecx, %cr3

    mov %cr0, %ecx
    or $((1 << 16) | (1 << 31)), %ecx
    mov %ecx, %cr0

    # Jump into the higher half
    jmp _start2
.size _start, . - _start

.section bootstrap_bss, "aw", @nobits
.align 4096
.global kernelPageDirectory

kernelPageDirectory:
    .skip 4096
pageTableBootstrap:
    .skip 4096
pageTableKernel:
    .skip 4096
pageTablePhysicalMemory:
	.skip 4096

.section .text
.type _start2, @function
_start2:
	mov $kernel_stack, %esp

	# Put the TSS address into the GDT
	mov $tss, %ecx
	mov %cx, gdt + 40 + 2
	shr $16, %ecx
	mov %cl, gdt + 40 + 4
	mov %ch, gdt + 40 + 7

	# load the GDT
	push $gdt
	pushw gdt_size
	lgdt (%esp)
	mov $0x10, %cx
	mov %cx, %ds
	mov %cx, %es
	mov %cx, %fs
	mov %cx, %gs
	mov %cx, %ss
	ljmp $0x8, $1f

1: 	mov $0x2B, %cx
	ltr %cx

	# load the IDT
	push $idt
	pushw idt_size
	lidt (%esp)
	
	mov $kernel_stack, %esp

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
.size _start2, . - _start2

.section .bss
.align 4096
.skip 4096

kernel_stack:
