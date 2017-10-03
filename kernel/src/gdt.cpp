#include <stdint.h>

struct gdt_entry
{
	uint16_t limit_low;
	uint16_t base_low;
	uint8_t base_middle;
	uint8_t access;
	uint8_t limit_high_flags;
	uint8_t base_high;
} __attribute__((__packed__));

#define GDT_ENTRY(base, limit, access, flags) \
{ \
	(limit) & 0xFFFF, \
	(base) & 0xFFFF, \
	((base) >> 16) & 0xFF, \
	(access) & 0xFF, \
	(((limit) >> 16) & 0x0F) | ((flags) & 0xF0), \
	((base) >> 24) & 0xFF \
}

#define GDT_PRESENT ((1 << 7) | (1 << 4))
#define GDT_RING0 0
#define GDT_EXECUTABLE (1 << 3)
#define GDT_READ_WRITE (1 << 1)

#define GDT_GRANULARITY_4K (1 << 7)
#define GDT_PROTECTED_MODE (1 << 6)

gdt_entry gdt[] = {
	// NUll Segment
	GDT_ENTRY(0,0,0,0),

	//Kernel Code Segment
	GDT_ENTRY(0, 0xFFFFFFF,GDT_PRESENT | GDT_RING0 | GDT_EXECUTABLE | GDT_READ_WRITE,
					GDT_GRANULARITY_4K | GDT_PROTECTED_MODE),

	//Kernel Data Segment
	GDT_ENTRY(0, 0xFFFFFFF, GDT_PRESENT | GDT_RING0 | GDT_READ_WRITE,
					GDT_GRANULARITY_4K | GDT_PROTECTED_MODE),
};

extern "C" {
	struct
	{
		uint16_t size;
		void* gdtp;
	} __attribute__((__packed__)) gdt_descriptor = {
		sizeof(gdt) -1,
		gdt
	};
}
