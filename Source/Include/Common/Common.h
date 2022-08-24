#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#define DisableInterrupts asm("cli"::)
#define EnableInterrupts asm("sti"::)

#define Outb(port,value) \
	asm volatile ("outb %%al, %%dx" :: "d" (port), "a" (value));

#define Inb(port) ({    \
	unsigned char _v;       \
	asm volatile ("inb %%dx, %%al" : "=a" (_v) : "d" (port)); \
        _v;     \
})

void *MemoryCopy(char *dst, char *src, int n);
void InitPic(void);