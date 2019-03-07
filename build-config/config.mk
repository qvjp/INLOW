# MIT License
# 
# Copyright (c) 2019 INLOW-OS
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# 

ARCH := i686
BUILD_DIR = $(TO_ROOT)/build
AR := $(ARCH)-elf-ar
CC := $(ARCH)-elf-gcc
CXX := $(ARCH)-elf-g++

# 在某些平台‘/’会被视为注释字符，也就不能在表达式中使用，--divide使‘/’变为正常字符，但不会影响
# 在行开头使用的‘/’进行注释的行为
ASFLAGS += -Wa,--divide -g
CXXFLAGS ?= -O0 -g
# 没有运行时支持，所以-fno-rtti、-fno-exceptions
CXXFLAGS += --sysroot=$(SYSROOT) -std=gnu++14 -ffreestanding -fno-exceptions -Wall -Wextra -fno-rtti
# CPPFLAGS中CPP是C PreProcessor的缩写
CPPFLAGS += -I $(TO_ROOT)/kernel/include -I $(SYSROOT)/usr/include
LDFLAGS ?= --sysroot=$(SYSROOT) -T $(TO_ROOT)/kernel/src/linker.ld -ffreestanding -nostdlib
LIBS ?= -lk -lgcc

MKRESCUE ?= grub-mkrescue

ISO ?= INLOW.iso

SYSROOT ?= $(TO_ROOT)/sysroot
