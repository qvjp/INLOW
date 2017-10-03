ARCH ?= i686

include ../build-config/config.mk

BUILD = ../build/$(ARCH)/kernel

ASPARAMETER += -Wa,--divide
CPPPARAMETER ?= -O2 -g
CPPPARAMETER += -std=gnu++14 -ffreestanding -fno-exceptions -fno-rtti -Wall -Wextra -I include
LDPARAMETER ?= -T linker.ld -ffreestanding -nostdlib
LIBS ?= -lgcc

CRTBEGIN_0 := $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtbegin.o)
CRTEND_0 := $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtend.o)

START_OBJ := $(BUILD)/arch/i686/crti.o $(CRTBEGIN_0)
END_OBJ := $(CRTEND_0) $(BUILD)/arch/i686/crtn.o

OBJ := addressspace.o \
		gdt.o \
		idt.o \
		interrupts.o \
		kernel.o \
		print.o
		
OBJ += arch/i686/interrupts.o \
	   arch/i686/loader.o

OBJ := $(START_OBJ) $(addprefix $(BUILD)/, $(OBJ)) $(END_OBJ)

all: $(BUILD)/kernel

$(BUILD)/kernel: $(OBJ)
	$(CPP) $(LDPARAMETER) -o $(BUILD)/kernel.elf $^ $(LIBS)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPPARAMETER) -c -o $@ $^

$(BUILD)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(CPP) $(ASPARAMETER) -c -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all clean
