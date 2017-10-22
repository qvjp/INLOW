REPO_ROOT = .
include ./build-config/config.mk
all: libc kernel utils iso

kernel: $(INCLUDE_DIR) $(LIB_DIR)
	$(MAKE) -C kernel

libc: $(INCLUDE_DIR)
	$(MAKE) -C libc

install-libc:
	$(MAKE) -C libc install

install-headers:
	$(MAKE) -C kernel install-headers
	$(MAKE) -C libc install-headers
INITRD = $(BUILD_DIR)/initrd/initrd.tar
iso: $(ISO)

$(ISO): $(BUILD_DIR)/kernel/kernel.elf $(INITRD)
	rm -rf $(BUILD_DIR)/isosrc
	cp -rf isosrc $(BUILD_DIR)
	cp -f $(BUILD_DIR)/kernel/kernel.elf $(BUILD_DIR)/isosrc
	cp -f $(INITRD) $(BUILD_DIR)/isosrc
	$(MKRESCUE) -o $@ $(BUILD_DIR)/isosrc

$(KERNEL): $(INCLUDE_DIR)
	$(MAKE) -C kernel

$(INITRD): $(BUILD_DIR)/utils/test
	@mkdir -p $(BUILD_DIR)/initrd
	echo Hello World! > $(BUILD_DIR)/initrd/hello
	cp -f $(BUILD_DIR)/utils/test $(BUILD_DIR)/initrd
	cd $(BUILD_DIR)/initrd && tar cvf initrd.tar --format=ustar hello test

qemu: $(ISO)
	qemu-system-i386 -cdrom $^

run: $(ISO)
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "INLOW" &

utils: $(INCLUDE_DIR)
	$(MAKE) -C utils

$(INCLUDE_DIR):
	$(MAKE) -C kernel install-headers
	$(MAKE) -C libc install-headers

$(LIB_DIR):
	$(MAKE) -C libc install-libs

clean:
	rm -rf ./build/
	rm -f $(ISO)

distclean:
	rm -rf ./build/ ./sysroot/
	rm -rf $(ISO)


.PHONY: all kernel libc install-headers install-libc iso qemu clean distclean

.PHONY: utils
