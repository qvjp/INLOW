
REPO_ROOT = ..
include ../build-config/config.mk

BUILD = ../build/$(ARCH)/libc

ARFLAGS := -crs
CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -std=gnu11 -ffreestanding -Wall -Wextra
CPPFLAGS += -I include -I -D_ALL_SOURCE
CXXFLAGS ?= -O2 -g
CXXFLAGS += --sysroot=$(SYSROOT) -std=gnu++14 -ffreestanding -Wall -Wextra

LIBC_FLAGS += -D__is_inlow_libc
LIBK_FLAGS += -D__is_inlow_libk

COMMON_OBJ := \
		cxx/new \
		cxx/delete \
		cxx/purevirtual \
		errno/errno \
		stdio/vcbprintf \
		stdlib/free \
		stdlib/malloc \
		stdlib/malloc-util \
		string/memcmp \
		string/memcpy \
		string/memmove \
		string/memset \
		string/strcmp \
		string/strcspn \
		string/strlen \
		string/strncmp

LIBC_OBJ := \
		$(COMMON_OBJ) \
		assert/assert \
		fcntl/open \
		fcntl/openat \
		stdio/fdopen \
		stdio/fgetc_unlocked \
		stdio/fgetc \
		stdio/fgets_unlocked \
		stdio/fgets \
		stdio/flockfile \
		stdio/fopen \
		stdio/fprintf \
		stdio/fputc_unlocked \
		stdio/fputc \
		stdio/fputs_unlocked \
		stdio/fputs \
		stdio/funlockfile \
		stdio/getc_unlocked \
		stdio/getc \
		stdio/getchar_unlocked \
		stdio/getchar \
		stdio/printf \
		stdio/putc_unlocked \
		stdio/putc \
		stdio/putchar_unlocked \
		stdio/putchar \
		stdio/puts \
		stdio/stderr \
		stdio/stdin \
		stdio/stdout \
		stdio/vfprintf_unlocked \
		stdio/vfprintf \
		stdlib/_Exit \
		stdlib/exit \
		sys/mman/mmap \
		sys/mman/munmap \
		unistd/_exit \
		unistd/read \
		unistd/write

LIBK_OBJ := $(COMMON_OBJ)

CRT_OBJ := \
		$(BUILD)/arch/i686/crt0.o \
		$(BUILD)/arch/i686/crti.o \
		$(BUILD)/arch/i686/crtn.o
LIBC_OBJ += \
			arch/i686/syscall

LIBC_OBJ := $(addprefix $(BUILD)/, $(addsuffix .o, $(LIBC_OBJ)))
LIBK_OBJ := $(addprefix $(BUILD)/, $(addsuffix .libk.o, $(LIBK_OBJ)))

all: $(BUILD)/libc.a $(BUILD)/libk.a $(CRT_OBJ)

-include $(LIBC_OBJ:.o=.d)
-include $(LIBK_OBJ:.o=.d)

install: install-headers install-libs

install-headers: 
	@mkdir -p $(INCLUDE_DIR)
	cp -rf include/. $(INCLUDE_DIR)

install-libs: $(BUILD)/libc.a $(BUILD)/libk.a $(CRT_OBJ)
	@mkdir -p $(SYSROOT)/usr/lib
	cp -f $^ $(SYSROOT)/usr/lib

$(BUILD)/libc.a: $(LIBC_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD)/libk.a: $(LIBK_OBJ)
	$(AR) $(ARFLAGS) $@ $^

$(BUILD)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LIBC_FLAGS) -MD -MP -c -o $@ $<

$(BUILD)/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LIBC_FLAGS) -MD -MP -c -o $@ $<

$(BUILD)/%.o: src/%.s
	@mkdir -p $(dir $@)
	$(CC) $(ASFLAGS) $(CPPFLAGS) $(LIBC_FLAGS) -c -o $@ $<

$(BUILD)/%.libk.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LIBK_FLAGS) -MD -MP -c -o $@ $<

$(BUILD)/%.libk.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LIBK_FLAGS) -MD -MP -c -o $@ $<

clean:
	rm -rf $(BUILD)

.PHONY: all install install-headers install-libs clean
