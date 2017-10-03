#include <inlow/kernel/interrupts.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/port.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA 0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA 0xA1

#define PIC_EOI 0x20

void Interrupts::initPic()
{
	outb(PIC1_COMMAND, 0x11);
	outb(PIC2_COMMAND, 0x11);

	outb(PIC1_DATA, 32);
	outb(PIC2_DATA, 40);

	outb(PIC1_DATA, 0x1);
	outb(PIC2_DATA, 0x1);
}

void Interrupts::enable()
{
	asm volatile ("sti");
}

extern "C" InterruptContext* handleInterrupt(InterruptContext* context)
{
	if (context->interrupt <= 31) //CPU Exception
	{
		Print::printf("Exception %u occurred!\n",context->interrupt);
		// Halt the CPU
		while (true)
				asm volatile ("cli; hlt");
	}
	else if(context->interrupt <= 47) // IRQ
	{
		Print::printf("IRQ %u occurred!\n", context->interrupt - 32);
		
		// Send End of Interrupt
		if (context->interrupt >= 40)
		{
			outb(PIC2_COMMAND, PIC_EOI);
		}	
		outb(PIC1_COMMAND, PIC_EOI);
	}
	else
	{
		Print::printf("Unknow interrupt %u!\n", context->interrupt);
	}
	return context;
}
