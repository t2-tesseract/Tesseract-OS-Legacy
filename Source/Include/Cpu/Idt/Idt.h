#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Common/Common.h>
// #include "Isr.h"

#define IdtBase  0x800
#define IdtSize  0xFF
#define IdtGate  0x8E00

struct IdtDescriptor;
struct IdtPointer;

void InitIdtDescriptor(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void InitIdt(void);
