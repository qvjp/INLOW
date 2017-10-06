ARCH ?= i686

include ../build-config/config.mk

BUILD = ../build/$(ARCH)/kernel

ASPARAMETER += -Wa,--divide
CXXPARAMETER ?= -O2 -g
CXXPARAMETER += --sysroot=$(SYSROOT) -std=gnu++14 -ffreestanding -fno-exceptions
CXXPARAMETER += -fno-rtti -Wall -Wextra
CPPPARAMETER += -I include -I $(SYSROOT)/usr/include
LDPARAMETER += --sysroot=$(SYSROOT) -T linker.ld -ffreestanding -nostdlib
LIBS += -lk -lgcc

CRTBEGIN_0 := $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtbegin.o)
CRTEND_0 := $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtend.o)

START_OBJ := $(BUILD)/arch/i686/crti.o $(CRTBEGIN_0)
END_OBJ := $(CRTEND_0) $(BUILD)/arch/i686/crtn.o

OBJ := addressspace.o \
		assert.o \
		gdt.o \
		idt.o \
		interrupts.o \
		kernel.o \
		print.o \
		physicalmemory.o
		
OBJ += arch/i686/interrupts.o \
	   arch/i686/loader.o

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
