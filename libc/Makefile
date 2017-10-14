
REPO_ROOT = ..
include ../build-config/config.mk

BUILD = ../build/$(ARCH)/libc

ARFLAGS := -crs
CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -std=gnu11 -ffreestanding -Wall -Wextra
CPPFLAGS += -I include -I $(SYSROOT)/usr/include
CXXFLAGS ?= -O2 -g
CXXFLAGS += --sysroot=$(SYSROOT) -std=gnu++14 -ffreestanding -Wall -Wextra

LIBC_FLAGS += -D__is_inlow_libc
LIBK_FLAGS += -D__is_inlow_libk

COMMON_OBJ := \
		cxx/new \
		string/memcmp \
		string/memcpy \
		string/memmove \
		string/memset

LIBC_OBJ := \
		$(COMMON_OBJ) \
		stdlib/_Exit \
		stdlib/exit \
		unistd/_exit

LIBK_OBJ := \
		$(COMMON_OBJ) \
		cxx/delete \
		stdlib/free \
		stdlib/malloc \
		stdlib/malloc-util

CRT_OBJ := \
		$(BUILD)/arch/i686/crt0.o \
		$(BUILD)/arch/i686/crti.o \
		$(BUILD)/arch/i686/crtn.o
LIBC_OBJ += \
			arch/i686/syscall

LIBC_OBJ := $(addprefix $(BUILD)/, $(addsuffix .o, $(LIBC_OBJ)))
LIBK_OBJ := $(addprefix $(BUILD)/, $(addsuffix .libk.o, $(LIBK_OBJ)))

all: $(BUILD)/libc.a $(BUILD)/libk.a $(CRT_OBJ)

install: install-headers install-libs

install-headers: $(SYSROOT)/usr/include

install-libs: $(BUILD)/libc.a $(BUILD)/libk.a $(CRT_OBJ)
	@mkdir -p $(SYSROOT)/usr/lib
	cp -f $^ $(SYSROOT)/usr/lib

$(SYSROOT)/usr/include: include $(REPO_ROOT)/kernel/include
	@mkdir -p $@
	@touch $@
	cp -rf $^ $(SYSROOT)/usr

$(BUILD)/libc.a: $(LIBC_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD)/libk.a: $(LIBK_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LIBC_FLAGS) -c -o $@ $^

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LIBC_FLAGS) -c -o $@ $^

$(BUILD)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $(CPPFLAGS) $(LIBC_FLAGS) -c -o $@ $^

$(BUILD)/%.libk.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LIBK_FLAGS) -c -o $@ $^

$(BUILD)/%.libk.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LIBK_FLAGS) -c -o $@ $^

clean:
	rm -rf $(BUILD)

.PHONY: all install install-headers install-libs clean
