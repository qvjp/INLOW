#include <inlow/kernel/interrupts.h>
#include <inlow/kernel/print.h>
#include <inlow/kernel/pit.h>
#include <inlow/kernel/port.h>

#define PIT_FREQUENCY 1193182 //Hz

#define PIT_PORT_CHANNEL0 0x40
#define PIT_PORT_MODE 0x43

#define PIT_MODE_RATE_GENERATOR 0x4
#define PIT_MODE_LOBYTE_HIBYTE 0x30

static const unsigned int frequency = 100;
static const uint16_t divider = PIT_FREQUENCY / frequency;
static const unsigned long nanoseconds = 1000000000L / PIT_FREQUENCY * divider;

static void irqHandler(int irq);

void Pit::initialize()
{
	Interrupts::irqHandlers[0] = irqHandler;
	outb(PIT_PORT_MODE, PIT_MODE_RATE_GENERATOR | PIT_MODE_LOBYTE_HIBYTE);
	outb(PIT_PORT_CHANNEL0, divider & 0xFF);
	outb(PIT_PORT_CHANNEL0, (divider >> 8) & 0xFF);
}

#define NUM_TIMERS 20
static Timer* timers[NUM_TIMERS] = {0};
void Pit::deregisterTimer(size_t index)
{
	timers[index] = nullptr;
}

size_t Pit::registerTimer(Timer* timer)
{
	for (size_t i = 0; i < NUM_TIMERS; i++)
	{
		if (!timers[i])
		{
			timers[i] = timer;
			return i;
		}
	}

	Print::printf("Error: Too many timers\n");
	while(true)
			asm volatile ("cli; hlt");
}

static void irqHandler(int)
{
	for (size_t i = 0; i < NUM_TIMERS; i++)
	{
		if (timers[i])
				timers[i]->advance(nanoseconds);
	}
}
