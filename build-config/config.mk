ARCH := i686
BUILD_DIR = $(REPO_ROOT)/build/$(ARCH)

ifneq ($(filter default undefined, $(origin CXX)),)
CXX = $(ARCH)-inlow-g++
endif

ifneq ($(filter default undefined, $(origin CC)),)
CC = $(ARCH)-inlow-gcc
endif

ifneq ($(filter default undefined, $(origin AR)),)
AR = $(ARCH)-inlow-ar
endif
MKRESCUE ?= grub-mkrescue

ISO ?= INLOW.iso

SYSROOT ?= $(REPO_ROOT)/sysroot
INCLUDE_DIR = $(SYSROOT)/include
LIB_DIR = $(SYSROOT)/lib
BIN_DIR = $(SYSROOT)/bin
INITRD = $(BUILD_DIR)/initrd.tar
