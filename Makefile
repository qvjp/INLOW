include ./build-config/config.mk
all: kernel iso

kernel:
	$(MAKE) -C kernel

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

.PHONY: all kernel iso qemu clean
