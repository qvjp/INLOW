#include <inlow/kernel/print.h>
#include <inlow/kernel/port.h>
#include <inlow/kernel/ps2keyboard.h>

PS2Keyboard::PS2Keyboard()
{
}

void PS2Keyboard::irqHandler()
{
	uint8_t data = inb(0x60);
	Print::printf("Scancode 0x%x received\n", data);
}
