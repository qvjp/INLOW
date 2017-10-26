ARCH ?= i686

REPO_ROOT = ..
include $(REPO_ROOT)/build-config/config.mk

BUILD = $(BUILD_DIR)/utils

CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -Wall -Wextra
CPPFLAGS += -D_INLOW_SOURCE

PROGRAMS = \
		   sh \
		   test \
		   test2

all: $(addprefix $(BUILD)/, $(PROGRAMS))

install: $(addprefix $(BUILD)/, $(PROGRAMS))
	@mkdir -p $(BIN_DIR)
	cp -f $^ $(BIN_DIR)
	
$(BUILD)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all install clean
