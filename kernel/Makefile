ARCH ?= i686
REPO_ROOT = ..

include $(REPO_ROOT)/build-config/config.mk
include $(REPO_ROOT)/build-config/version.mk

BUILD = ../build/$(ARCH)/kernel

CXXPARAMETER ?= -O2 -g
CXXPARAMETER += --sysroot=$(SYSROOT) -std=gnu++14 -ffreestanding -fno-exceptions
CXXPARAMETER += -fno-rtti -Wall -Wextra
CPPPARAMETER += -I include -DINLOW_VERSION=\"$(VERSION)\"
CPPPARAMETER += -D__is_inlow_kernel -D_INLOW_SOURCE
LDPARAMETER += --sysroot=$(SYSROOT) -T linker.ld -ffreestanding -nostdlib
LIBS += -lk -lgcc

CRTI_0 = $(shell $(CXX) $(CXXPARAMETER) -print-file-name=crti.o)
CRTBEGIN_0 = $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtbegin.o)
CRTEND_0 = $(shell $(CPP) $(CPPPARAMETER) -print-file-name=crtend.o)
CRTN_0 = $(shell $(CXX) $(CXXPARAMETER) -print-file-name=crtn.o)

START_OBJ = $(CRTI_0) $(CRTBEGIN_0)
END_OBJ = $(CRTEND_0) $(CRTN_0) 

OBJ = addressspace.o \
		assert.o \
		conf.o \
		directory.o \
		file.o \
		filedescription.o \
		gdt.o \
		idt.o \
		initrd.o \
		interrupts.o \
		kernel.o \
		keyboard.o \
		print.o \
		memorysegment.o \
		physicalmemory.o \
		pit.o \
		process.o \
		ps2.o \
		ps2keyboard.o \
		syscall.o \
		terminal.o \
		timer.o \
		vgaterminal.o \
		vnode.o
		
OBJ += arch/i686/interrupts.o \
	   arch/i686/loader.o \
	   arch/i686/syscall.o


all: $(BUILD)/kernel

OBJ := $(addprefix $(BUILD)/, $(OBJ))
-include $(OBJ:.o=.d)

OBJ := $(START_OBJ) $(OBJ) $(END_OBJ)

install-headers:
	@mkdir -p $(INCLUDE_DIR)
	cp -rf include/. $(INCLUDE_DIR)
	touch $(SYSROOT)

$(BUILD)/kernel: $(OBJ)
	$(CXX) $(LDPARAMETER) -o $(BUILD)/kernel.elf $^ $(LIBS)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXPARAMETER) $(CPPPARAMETER) -MD -MP -c -o $@ $<

$(BUILD)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(CXX) $(ASPARAMETER) $(CPPPARAMETER) -MD -MP -c -o $@ $<

clean:
	rm -rf $(BUILD)

.PHONY: all clean
