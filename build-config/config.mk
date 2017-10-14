ARCH := i686
BUILD_DIR = $(REPO_ROOT)/build/$(ARCH)
CXX := $(ARCH)-inlow-g++
CC := $(ARCH)-inlow-gcc
AR := $(ARCH)-inlow-ar
MKRESCUE ?= grub-mkrescue

ISO ?= INLOW.iso

SYSROOT ?= ../sysroot
