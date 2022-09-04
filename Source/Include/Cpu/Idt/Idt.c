#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "Idt.h"

void DefaultInt(void);
void Irq0(void);
void Irq1(void);

struct IdtPointer KernelIdtPointer;
struct IdtDescriptor KernelIdt[IdtSize];

void InitIdtDescriptor(uint16_t select, uint32_t offset, uint16_t type, struct IdtDescriptor *Descriptor)
{
	Descriptor->offset0_15 = (offset & 0xffff);
	Descriptor->select = select;
	Descriptor->type = type;
	Descriptor->offset16_31 = (offset & 0xffff0000) >> 16;
	return;
}

void InitIdt(void){
	int i;

	for (i = 0; i < IdtSize; i++) 
		InitIdtDescriptor(0x08, (uint32_t) DefaultInt, IdtGate, &KernelIdt[i]);

	InitIdtDescriptor(0x08, (uint32_t) Irq0, IdtGate, &KernelIdt[32]); // Clock
	InitIdtDescriptor(0x08, (uint32_t) Irq1, IdtGate, &KernelIdt[33]); // Keyboard

	KernelIdtPointer.limit = IdtSize * 8;
	KernelIdtPointer.base = IdtBase;

	MemoryCopy((char *) KernelIdtPointer.base, (char *) KernelIdt, KernelIdtPointer.limit);

	asm("lidtl (KernelIdtPointer)");
}


