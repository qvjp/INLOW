#include <inlow/kernel/addressspace.h> /* AddressSpace_initialize() */
#include <inlow/kernel/physicalmemory.h>
#include <inlow/kernel/print.h> /* printf() */
#include <inlow/kernel/isr.h> /* isrs_install() */
#include <inlow/kernel/irq.h> /* irq_install() irq_enable()*/

void kernel_main(uint32_t magic, paddr_t multibootAddress)
{
    printf("Welcome to INLOW!\n");

    isrs_install();
    irq_install();
    irq_enable();
    printf("Interrupts enabled!\n");

    AddressSpace_initialize();
    printf("Address Space Initialized!\n");
    printf("kernel_main: 0x%x\n", kernelSpace);

    struct multiboot_info* multiboot = (struct multiboot_info*) kernelSpace->map(multibootAddress, 0x1 | 0x2);
    Physicalmemory_initialize(multiboot);
    kernelSpace->unmap((vaddr_t) multiboot);

    // printf("%d", 1/0);
    while(1);
}
