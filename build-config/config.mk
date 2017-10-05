ARCH := i686
BUILD_DIR = ./build/$(ARCH)
CXX := $(ARCH)-elf-g++
CC := $(ARCH)-elf-gcc
AR := $(ARCH)-elf-ar
MKRESCUE ?= grub-mkrescue

ISO ?= INLOW.iso

SYSROOT ?= ../sysroot