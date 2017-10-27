#include <inlow/kernel/pit.h>
#include <inlow/kernel/port.h>

#define PIT_FREQUENCY 1193182 //Hz

#define PIT_PORT_CHANNEL0 0x40
#define PIT_PORT_MODE 0x43

#define PIT_MODE_RATE_GENERATOR 0x4
#define PIT_MODE_LOBYTE_HIBYTE 0x30

void Pit::initialize()
{
	outb(PIT_PORT_MODE, PIT_MODE_RATE_GENERATOR | PIT_MODE_LOBYTE_HIBYTE);

	const uint16_t divider = PIT_FREQUENCY / 100;
	outb(PIT_PORT_CHANNEL0, divider & 0xFF);
	outb(PIT_PORT_CHANNEL0, (divider >> 8) & 0xFF);
}
