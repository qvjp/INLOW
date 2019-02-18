# MIT License
# 
# Copyright (c) 2019 INLOW-OS
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
# 

TO_ROOT = .
include $(TO_ROOT)/build-config/config.mk

all: kernel strip-debug iso

kernel:
	$(MAKE) -C kernel

iso: $(ISO)

# 为调试生成kernel.sym，并删除kernel.elf中的多余调试信息
strip-debug:
	objcopy --only-keep-debug $(BUILD_DIR)/$(ARCH)/kernel/kernel.elf $(BUILD_DIR)/kernel.sym
	objcopy --strip-debug $(BUILD_DIR)/$(ARCH)/kernel/kernel.elf

$(ISO): $(BUILD_DIR)/$(ARCH)/kernel/kernel.elf
	mkdir iso
	mkdir iso/boot
	mkdir iso/boot/grub
	cp $(BUILD_DIR)/$(ARCH)/kernel/kernel.elf iso/boot/kernel.elf
	echo 'set timeout=0'                   >  iso/boot/grub/grub.cfg
	echo 'set default=0'                   >> iso/boot/grub/grub.cfg
	echo ''                                >> iso/boot/grub/grub.cfg
	echo 'menuentry "INLOW" {'             >> iso/boot/grub/grub.cfg
	echo '    multiboot /boot/kernel.elf'  >> iso/boot/grub/grub.cfg
	echo '    boot'                        >> iso/boot/grub/grub.cfg
	echo '}'                               >> iso/boot/grub/grub.cfg
	grub-mkrescue --output=$(ISO) iso
	rm -rf iso

qemu: $(ISO)
	qemu-system-i386 -cdrom $^

qemu-dbg: $(ISO)
	qemu-system-i386 -cdrom $^ -S -s

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(ISO)

.PHONY: all kernel iso qemu clean
