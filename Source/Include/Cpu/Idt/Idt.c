#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include "Idt.h"
#include "Isr.h"

extern void LoadIdt();

struct IdtDescriptor{
	unsigned short base_lo;
    unsigned short sel;        /* Our kernel segment goes here! */
    unsigned char always0;     /* This will ALWAYS be set to 0! */
    unsigned char flags;       /* Set using the above table! */
    unsigned short base_hi;
} __attribute__ ((packed));

struct IdtPointer{
	uint16_t limit;
	uint32_t base;
} __attribute__ ((packed));

struct IdtPointer KernelIdtPointer;
struct IdtDescriptor KernelIdt[IdtSize];

void InitIdtDescriptor(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags){
    KernelIdt[num].base_lo = (base & 0xFFFF);
    KernelIdt[num].base_hi = (base >> 16) & 0xFFFF;
    KernelIdt[num].sel = sel;
    KernelIdt[num].always0 = 0;
    KernelIdt[num].flags = flags;
}

void InitIdt(void){
    // InitIdtDescriptor(32, (uint32_t)Irq0, 0x08, 0x8E);
    // InitIdtDescriptor(33, (uint32_t)Irq1, 0x08, 0x8E);
    // InitIdtDescriptor(34, (uint32_t)Irq2, 0x08, 0x8E);
    // InitIdtDescriptor(35, (uint32_t)Irq3, 0x08, 0x8E);
    // InitIdtDescriptor(36, (uint32_t)Irq4, 0x08, 0x8E);
    // InitIdtDescriptor(37, (uint32_t)Irq5, 0x08, 0x8E);
    // InitIdtDescriptor(38, (uint32_t)Irq6, 0x08, 0x8E);
    // InitIdtDescriptor(39, (uint32_t)Irq7, 0x08, 0x8E);
    // InitIdtDescriptor(40, (uint32_t)Irq8, 0x08, 0x8E);
    // InitIdtDescriptor(41, (uint32_t)Irq9, 0x08, 0x8E);
    // InitIdtDescriptor(42, (uint32_t)Irq10, 0x08, 0x8E);
    // InitIdtDescriptor(43, (uint32_t)Irq11, 0x08, 0x8E);
    // InitIdtDescriptor(44, (uint32_t)Irq12, 0x08, 0x8E);
    // InitIdtDescriptor(45, (uint32_t)Irq13, 0x08, 0x8E);
    // InitIdtDescriptor(46, (uint32_t)Irq14, 0x08, 0x8E);
    // InitIdtDescriptor(47, (uint32_t)Irq15, 0x08, 0x8E);

	KernelIdtPointer.limit = (sizeof (struct IdtDescriptor) * 256) - 1;
    KernelIdtPointer.base = &KernelIdt;
    MemorySet(&KernelIdt, 0, sizeof(struct IdtDescriptor) * 256);
    LoadIdt();
}


