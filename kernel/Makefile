ARCH ?= i686

include ../build-config/config.mk

BUILD = ../build/$(ARCH)/kernel

CPPPARAMETER ?= -O2 -g
CPPPARAMETER += -std=gnu++14 -ffreestanding -fno-exceptions -fno-rtti -Wall -Wextra
LDPARAMETER ?= -T linker.ld -ffreestanding -nostdlib
LIBS ?= -lgcc

OBJ := kernel.o
OBJ += arch/i686/start.o

OBJ := $(addprefix $(BUILD)/, $(OBJ))
all: $(BUILD)/kernel

$(BUILD)/kernel: $(OBJ)
	$(CPP) $(LDPARAMETER) -o $(BUILD)/kernel.elf $^ $(LIBS)

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CPP) $(CPPPARAMETER) -c -o $@ $^

$(BUILD)/%.o: src/%.S
	@mkdir -p $(dir $@)
	$(CPP) $(ASPARAMETER) -c -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all clean
