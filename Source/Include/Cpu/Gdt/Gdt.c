#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#define __GDT__
#include "Gdt.h"

void InitGdtDescriptor(uint32_t Base, uint32_t Limit, uint8_t Access, uint8_t Other, struct GdtDescriptor*  Descriptor){
    Descriptor->lim0_15 = (Limit & 0xffff);
    Descriptor->base0_15 = (Base & 0xffff);
    Descriptor->base16_23 = (Base & 0xff0000) >> 16;
    Descriptor->access = Access;
    Descriptor->lim16_19 = (Limit & 0xf0000) >> 16;
    Descriptor->other = (Other & 0xf);
    Descriptor->base24_31 = (Base & 0xff000000) >> 24;

    return;
}

void InitGdt(){

    InitGdtDescriptor(0x0, 0x0, 0x0, 0x0, &KernelGdt[0]);
	InitGdtDescriptor(0x0, 0xFFFFF, 0x9B, 0x0D, &KernelGdt[1]);	/* code */
	InitGdtDescriptor(0x0, 0xFFFFF, 0x93, 0x0D, &KernelGdt[2]);	/* data */
	InitGdtDescriptor(0x0, 0x0, 0x97, 0x0D, &KernelGdt[3]);		/* stack */

    KernelGdtPointer.limit = GdtSize * 8;
    KernelGdtPointer.base = GdtBase;

    MemoryCopy((char*) KernelGdtPointer.base, (char*) KernelGdt, KernelGdtPointer.limit);

    asm("lgdtl (KernelGdtPointer)");

    asm("   movw $0x10, %ax \n \
        movw %ax, %ds       \n \
        movw %ax, %es       \n \
        movw %ax, %fs       \n \
        movw %ax, %gs       \n \
        ljmp $0x08, $next   \n \
        next:               \n");
}
