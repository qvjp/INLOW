ARCH := i686
BUILD_DIR = $(TO_ROOT)/build/$(ARCH)
GCC := $(ARCH)-elf-gcc

CFLAGS ?= -O2 -g
CFLAGS += -std=gnu99 -ffreestanding -fno-exceptions -Wall -Wextra -I $(TO_ROOT)/kernel/include
LDFLAGS ?= -T $(TO_ROOT)/kernel/src/linker.ld -ffreestanding -nostdlib
LIBS ?= -lgcc

MKRESCUE ?= grub-mkrescue

ISO ?= INLOW.iso
