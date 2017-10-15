ARCH ?= i686

include ../build-config/config.mk

BUILD = ../build/$(ARCH)/kernel

ASPARAMETER += -Wa,--divide
CXXPARAMETER ?= -O2 -g
CXXPARAMETER += --sysroot=$(SYSROOT) -std=gnu++14 -ffreestanding -fno-exceptions
CXXPARAMETER += -fno-rtti -Wall -Wextra
CPPPARAMETER += -I include -I $(SYSROOT)/usr/include
CPPPARAMETER += -D__is_inlow_kernel -D_DENNIX_SOURCE
LDPARAMETER += --sysroot=$(SYSROOT) -T linker.ld -ffreestanding -nostdlib
LIBS += -lk -lgcc

CRTI_0 := $(shell $(CXX) $(CXXPARAMETER) -print-file-name=crti.o)
CRTBEGIN_0 := $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtbegin.o)
CRTEND_0 := $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtend.o)
CRTN_0 := $(shell $(CXX) $(CXXPARAMETER) -print-file-name=crtn.o)

START_OBJ := $(CRTI_0) $(CRTBEGIN_0)
END_OBJ := $(CRTEND_0) $(CRTN_0) 

OBJ := addressspace.o \
		assert.o \
		filedescription.o \
		gdt.o \
		idt.o \
		interrupts.o \
		kernel.o \
		print.o \
		physicalmemory.o \
		process.o \
		syscall.o \
		terminal.o \
		vnode.o
		
OBJ += arch/i686/interrupts.o \
	   arch/i686/loader.o \
	   arch/i686/syscall.o

OBJ := $(START_OBJ) $(addprefix $(BUILD)/, $(OBJ)) $(END_OBJ)

all: $(BUILD)/kernel

$(BUILD)/kernel: $(OBJ)
	$(CXX) $(LDPARAMETER) -o $(BUILD)/kernel.elf $^ $(LIBS)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXPARAMETER) $(CPPPARAMETER) -c -o $@ $^

$(BUILD)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(CXX) $(ASPARAMETER) $(CPPPARAMETER) -c -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all clean
