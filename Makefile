REPO_ROOT = .
include ./build-config/config.mk
all: libc kernel utils iso

kernel: $(INCLUDE_DIR) $(LIB_DIR)
	$(MAKE) -C kernel

libc: $(INCLUDE_DIR)
	$(MAKE) -C libc

install-all: install-headers install-libc install-utils

install-headers:
	$(MAKE) -C kernel install-headers
	$(MAKE) -C libc install-headers

install-libc:
	$(MAKE) -C libc install-libs

install-utils:
	$(MAKE) -C utils install

iso: $(ISO)

$(ISO): $(BUILD_DIR)/kernel/kernel.elf $(INITRD)
	rm -rf $(BUILD_DIR)/isosrc
	cp -rf isosrc $(BUILD_DIR)
	cp -f $(BUILD_DIR)/kernel/kernel.elf $(BUILD_DIR)/isosrc
	cp -f $(INITRD) $(BUILD_DIR)/isosrc
	$(MKRESCUE) -o $@ $(BUILD_DIR)/isosrc

$(KERNEL): $(INCLUDE_DIR)
	$(MAKE) -C kernel

$(INITRD): $(SYSROOT)
	echo Hello World! > $(SYSROOT)/hello
	cd $(SYSROOT) && tar cvf ../$(INITRD) --format=ustar *

qemu: $(ISO)
	qemu-system-i386 -cdrom $^

run: $(ISO)
	(killall VirtualBox && sleep 1) || true
	VirtualBox --startvm "INLOW" &

utils: $(INCLUDE_DIR)
	$(MAKE) -C utils

$(SYSROOT): $(INCLUDE_DIR) $(LIB_DIR) $(BIN_DIR)

$(BIN_DIR):
	$(MAKE) -C utils install

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


.PHONY: all kernel libc install-all install-headers install-libc

.PHONY: install_utils iso qemu utils clean distclean
