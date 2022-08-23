#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Common/Common.h>

#define Base  0x800	
#define Size  0xFF	
#define Gate  0x8E00	

struct IdtDescriptor{
	uint16_t offset0_15;
	uint16_t select;
	uint16_t type;
	uint16_t offset16_31;
} __attribute__ ((packed));

struct IdtPointer{
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));

void InitIdtDescriptor(uint16_t, uint32_t, uint16_t, struct IdtDescriptor*);
void InitIdt(void);

