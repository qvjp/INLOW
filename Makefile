REPO_ROOT = .
include ./build-config/config.mk
all: install-headers libc install-libc kernel utils iso

kernel:
	$(MAKE) -C kernel

libc:
	$(MAKE) -C libc

install-libc:
	$(MAKE) -C libc install

install-headers:
	$(MAKE) -C libc install-headers

iso: $(ISO)

$(ISO): $(BUILD_DIR)/kernel/kernel.elf
	rm -rf $(BUILD_DIR)/isosrc
	cp -rf isosrc $(BUILD_DIR)
	cp -f $(BUILD_DIR)/kernel/kernel.elf $(BUILD_DIR)/isosrc
	cp -f $(BUILD_DIR)/utils/test $(BUILD_DIR)/isosrc
	$(MKRESCUE) -o $@ $(BUILD_DIR)/isosrc


qemu: $(ISO)
	qemu-system-i386 -cdrom $^

utils:
	$(MAKE) -C utils

clean:
	rm -rf ./build/
	rm -f $(ISO)

distclean:
	rm -rf ./build/ ./sysroot/
	rm -rf $(ISO)


.PHONY: all kernel libc install-headers install-libc iso qemu clean distclean

.PHONY: utils
