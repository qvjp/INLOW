
REPO_ROOT = ..
include ../build-config/config.mk

BUILD = ../build/$(ARCH)/libc

ARFLAGS = -crs
CFLAGS ?= -O2 -g
CFLAGS += --sysroot=$(SYSROOT) -std=gnu11 -ffreestanding -Wall -Wextra
CPPFLAGS += -I include -I -D_ALL_SOURCE
CXXFLAGS ?= -O2 -g
CXXFLAGS += --sysroot=$(SYSROOT) -std=gnu++14 -ffreestanding -Wall -Wextra

LIBC_FLAGS += -D__is_inlow_libc
LIBK_FLAGS += -D__is_inlow_libk

COMMON_OBJ = \
		ctype/isspace \
		cxx/new \
		cxx/delete \
		cxx/purevirtual \
		errno/errno \
		getopt/getopt \
		getopt/getopt_long \
		libgen/basename \
		libgen/dirname \
		sched/sched_yield \
		signal/sigaddset \
		signal/sigdelset \
		signal/sigemptyset \
		signal/sigfillset \
		signal/sigismember \
		stdio/vcbprintf \
		stdlib/calloc \
		stdlib/free \
		stdlib/malloc \
		stdlib/malloc-util \
		stdlib/rand \
		stdlib/realloc \
		stdlib/reallocarray \
		stdlib/strtol \
		stdlib/strtoul \
		string/memcmp \
		string/memcpy \
		string/memmove \
		string/memset \
		string/stpcpy \
		string/strcmp \
		string/strcpy \
		string/strchr \
		string/strcspn \
		string/strdup \
		string/strerror \
		string/strlcpy \
		string/strlen \
		string/strncmp \
		string/strndup \
		string/strnlen \
		string/strrchr \
		string/strsignal \
		string/strspn \
		string/strtok \
		time/timegm

LIBC_OBJ = \
		$(COMMON_OBJ) \
		assert/assert \
		dirent/closedir \
		dirent/fdopendir \
		dirent/opendir \
		dirent/readdir \
		dirent/rewinddir \
		err/err \
		err/errc \
		err/errx \
		err/verr \
		err/verrc \
		err/verrx \
		err/vwarn \
		err/vwarnc \
		err/vwarnx \
		err/warn \
		err/warnc \
		err/warnx \
		errno/initProgname \
		fcntl/open \
		fcntl/openat \
		signal/kill \
		signal/raise \
		signal/sigaction \
		signal/signal \
		stdio/clearerr_unlocked \
		stdio/clearerr \
		stdio/fclose \
		stdio/fdopen \
		stdio/feof_unlocked \
		stdio/feof \
		stdio/ferror_unlocked \
		stdio/ferror \
		stdio/fflush \
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
		stdio/fwrite_unlocked \
		stdio/fwrite \
		stdio/getc_unlocked \
		stdio/getc \
		stdio/getchar_unlocked \
		stdio/getchar \
		stdio/getdelim \
		stdio/getline \
		stdio/printf \
		stdio/putc_unlocked \
		stdio/putc \
		stdio/putchar_unlocked \
		stdio/putchar \
		stdio/puts \
		stdio/remove \
		stdio/rename \
		stdio/renameat \
		stdio/stderr \
		stdio/stdin \
		stdio/stdout \
		stdio/vfprintf_unlocked \
		stdio/vfprintf \
		stdlib/abort \
		stdlib/atexit \
		stdlib/canonicalize_file_name \
		stdlib/_Exit \
		stdlib/exit \
		stdlib/getenv \
		stdlib/setenv \
		stdlib/unsetenv \
		sys/mman/mmap \
		sys/mman/munmap \
		sys/stat/fstat \
		sys/stat/fstatat \
		sys/stat/lstat \
		sys/stat/mkdir \
		sys/stat/mkdirat \
		sys/stat/stat \
		sys/utsname/uname \
		sys/wait/wait \
		sys/wait/waitpid \
		termios/tcgetattr \
		termios/tcsetattr \
		time/clock_gettime \
		time/nanosleep \
		time/time \
		unistd/access \
		unistd/chdir \
		unistd/close \
		unistd/confstr \
		unistd/environ \
		unistd/execl \
		unistd/execv \
		unistd/execve \
		unistd/_exit \
		unistd/fchdirat \
		unistd/fork \
		unistd/getcwd \
		unistd/gethostname \
		unistd/getlogin \
		unistd/getpid \
		unistd/link \
		unistd/linkat \
		unistd/read \
		unistd/regfork \
		unistd/rmdir \
		unistd/sleep \
		unistd/symlink \
		unistd/symlinkat \
		unistd/unlink \
		unistd/unlinkat \
		unistd/write

LIBK_OBJ = $(COMMON_OBJ)

CRT_OBJ = \
		$(BUILD)/arch/i686/crt0.o \
		$(BUILD)/arch/i686/crti.o \
		$(BUILD)/arch/i686/crtn.o
LIBC_OBJ += \
			arch/i686/rfork \
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
	touch $(SYSROOT)

install-libs: $(BUILD)/libc.a $(BUILD)/libk.a $(CRT_OBJ)
	@mkdir -p $(LIB_DIR)
	cp -f $^ $(LIB_DIR)
	touch $(SYSROOT)

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
