#include <inlow/kernel/addressspace.h> /* AddressSpace_initialize() */
#include <inlow/kernel/print.h> /* printf() */
#include <inlow/kernel/isr.h> /* isrs_install() */
#include <inlow/kernel/irq.h> /* irq_install() irq_enable()*/

void kernel_main()
{
    printf("Welcome to INLOW!\n");
    AddressSpace_initialize();
    printf("Address Space Initialized!\n");
    // isrs_install();
    // irq_install();
    // irq_enable();
    // printf("Interrupts enabled!\n");
    // printf("%d", 1/0);
    while(1);
}
