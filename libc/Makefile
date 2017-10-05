
include ../build-config/config.mk

BUILD = ../build/$(ARCH)/libc

ARFLAGS := -crs
CFLAGS ?= -O2 -g
CFLAGS += -std=gnu11 -ffreestanding -Wall -Wextra
CPPFLAGS += -I include

LIBK_OBJ := \
		stdlib/free \
		stdlib/malloc \
		stdlib/malloc-util

LIBK_OBJ := $(addprefix $(BUILD)/, $(addsuffix .libk.o, $(LIBK_OBJ)))

all: $(BUILD)/libk.a

install: install-headers install-libk

install-headers: $(SYSROOT)/usr/include

install-libk: $(BUILD)/libk.a
	@mkdir -p $(SYSROOT)/usr/lib
	cp -f $^ $(SYSROOT)/usr/lib

$(SYSROOT)/usr/include: include
	@mkdir -p $@
	@touch $@
	cp -rf include $(SYSROOT)/usr

$(BUILD)/libk.a: $(LIBK_OBJ)
	$(AR) $(ARFLAGS) $@ $^


$(BUILD)/%.libk.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all install install-headers install-libk clean

