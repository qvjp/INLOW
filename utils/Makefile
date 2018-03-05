ARCH ?= i686

REPO_ROOT = ..
include $(REPO_ROOT)/build-config/config.mk
include $(REPO_ROOT)/build-config/version.mk

BUILD = $(BUILD_DIR)/utils

CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -Wall -Wextra
CPPFLAGS += -D_INLOW_SOURCE -DINLOW_VERSION=\"$(VERSION)\"

BIN_PROGRAMS = \
		   cat \
		   clear \
		   cp \
		   date \
		   echo \
		   false \
		   ln \
		   ls \
		   mkdir \
		   mv \
		   pwd \
		   rm \
		   sh \
		   sleep \
		   snake \
		   true \
		   uname \
		   version \
		   miv

SBIN_PROGRAMS = \
				init

PROGRAMS = $(BIN_PROGRAMS) $(SBIN_PROGRAMS)

all: SuperRun $(addprefix $(BUILD)/, $(PROGRAMS))

SuperRun: install
	$(MAKE) -C fly

install: install-bin install-sbin

install-bin: $(addprefix $(BUILD)/, $(BIN_PROGRAMS))
	@mkdir -p $(BIN_DIR)
	cp -f $^ $(BIN_DIR)
	touch $(SYSROOT)
	
install-sbin: $(addprefix $(BUILD)/, $(SBIN_PROGRAMS))
	@mkdir -p $(SBIN_DIR)
	cp -f $^ $(SBIN_DIR)
	touch $(SYSROOT)
	
$(BUILD)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all install install-bin install-sbin clean
