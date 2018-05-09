TO_ROOT = ..
include $(TO_ROOT)/build-config/config.mk

BUILD = $(BUILD_DIR)/kernel

CRTBEGIN_0 := $(shell $(GCC) $(CFLAGS) -print-file-name=crtbegin.o)
CRTEND_0 := $(shell $(GCC) $(CFLAGS) -print-file-name=crtend.o)
START_OBJ := $(BUILD)/arch/i686/crti.o $(CRTBEGIN_0)
END_OBJ := $(CRTEND_0) $(BUILD)/arch/i686/crtn.o

OBJ := kernel.o
OBJ += arch/i686/loader.o

OBJ := $(START_OBJ) $(addprefix $(BUILD)/, $(OBJ)) $(END_OBJ)
all: $(BUILD)/kernel

$(BUILD)/kernel: $(OBJ)
	$(GCC) $(LDFLAGS) -o $(BUILD)/kernel.elf $^ $(LIBS)

$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(GCC) $(CFLAGS) -c -o $@ $^

$(BUILD)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(GCC) $(ASFLAGS) -c -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all clean
