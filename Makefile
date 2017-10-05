include ./build-config/config.mk
all: install-headers libc install-libc kernel iso

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
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $(BUILD_DIR)/kernel/kernel.elf iso/boot/kernel.elf
	echo 'set timeout=0' 					  > iso/boot/grub/grub.cfg
	echo 'set default=0' 					 >> iso/boot/grub/grub.cfg
	echo '' 								 >> iso/boot/grub/grub.cfg
	echo 'menuentry "INLOW" {'			   	 >> iso/boot/grub/grub.cfg
	echo '	multiboot /boot/kernel.elf' 	 >> iso/boot/grub/grub.cfg
	echo '  boot' 							 >> iso/boot/grub/grub.cfg
	echo '}' 								 >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$(ISO) iso
	rm -rf iso

qemu: $(ISO)
	qemu-system-i386 -cdrom $^

clean:
	rm -rf ./build/
	rm -f $(ISO)

distclean:
	rm -rf ./build/ ./sysroot/
	rm -rf $(ISO)


.PHONY: all kernel libc install-headers install-libc iso qemu clean distclean
