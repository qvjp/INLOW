ENTRY(_start);

/* This is the kernel space starts address */
KERNEL_VIRTUAL = 0xC0000000;

SECTIONS {
	. = 0x100000;

	bootstrapBegin = .;

	bootstrap_text : {
		*(multiboot)
		*(bootstrap_text)
	}

	bootstrap_bss ALIGN(4096) : {
		*(bootstrap_bss)
	}

	bootstrapEnd = ALIGN(4096);
	numBootstrapPages = (bootstrapEnd - bootstrapBegin) / 0x1000;

	. += KERNEL_VIRTUAL;

	kernelVirtualBegin = ALIGN(4096);
	kernelPhysicalBegin = kernelVirtualBegin - KERNEL_VIRTUAL;

	.text ALIGN(4096) : AT(ADDR(.text) - KERNEL_VIRTUAL) {
		*(.text)
	}

	.rodata ALIGN(4096) : AT(ADDR(.rodata) - KERNEL_VIRTUAL) {
		*(.rodata)
	}

	.data ALIGN(4096) : AT(ADDR(.data) - KERNEL_VIRTUAL) {
		*(.data)
	}

	.bss ALIGN(4096) : AT(ADDR(.bss) - KERNEL_VIRTUAL) {
		*(.bss)
	}

	kernelVirtualEnd = ALIGN(4096);
	kernelPhysicalEnd = kernelVirtualEnd - KERNEL_VIRTUAL;
	numKernelPages = (kernelVirtualEnd - kernelVirtualBegin) / 0x1000;
}
