#include <inlow/kernel/print.h> /* printf() */
#include <inlow/kernel/isr.h>
#include <inlow/kernel/irq.h>

void kernel_main()
{
    printf("Welcome to INLOW!\n");

    isrs_install();
    irq_install();
    irq_enable();
    printf("Interrupts enabled!\n");
    // printf("%d", 1/0);
    while(1);
}
