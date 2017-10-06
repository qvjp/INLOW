ARCH ?= i686

REPO_ROOT = ..
include $(REPO_ROOT)/build-config/config.mk

BUILD = $(BUILD_DIR)/utils
LDFLAGS += --sysroot=$(SYSROOT) -nostdlib

PROGRAMS = \
		   test

all: $(addprefix $(BUILD)/, $(PROGRAMS))
	
$(BUILD)/%.o: %.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) -c -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all clean
