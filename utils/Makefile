ARCH ?= i686

REPO_ROOT = ..
include $(REPO_ROOT)/build-config/config.mk

BUILD = $(BUILD_DIR)/utils

CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -Wall -Wextra
CPPFLAGS += -D_INLOW_SOURCE

PROGRAMS = \
		   test

all: $(addprefix $(BUILD)/, $(PROGRAMS))
	
$(BUILD)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all clean
