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

void Outw(unsigned short port, unsigned short value);
static inline void IoWait(void);
void Delay(uint16_t Ms);
void *MemoryCopy(char *dst, char *src, int n);
// size_t StringLength(const char* String);
char* StringNCat(char * String1, char * String2, uint32_t n);
uint32_t StringLength(const char *String) ;
uint32_t __StringLength(const char *String, uint32_t MaxLen);
bool Backspace(char Buffer[]);
void Append(char s[], char n);
void InitPic(void);
