ARCH ?= i686

REPO_ROOT = ../..
include $(REPO_ROOT)/build-config/config.mk
include $(REPO_ROOT)/build-config/version.mk

BUILD = $(BUILD_DIR)/utils

CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -Wall -Wextra
CPPFLAGS += -D_INLOW_SOURCE -DINLOW_VERSION=\"$(VERSION)\"

main: *.c
	$(CC) $(CFLAGS) $(CPPFLAGS) $^ -o $(BIN_DIR)/fly
