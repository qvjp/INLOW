ARCH := i686
BUILD_DIR = ./build/$(ARCH)
CPP := $(ARCH)-elf-g++
MKRESCUE ?= grub-mkrescue

ISO ?= INLOW.iso
