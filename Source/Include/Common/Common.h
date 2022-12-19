#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

#define DisableInterrupts asm("cli"::)
#define EnableInterrupts asm("sti"::)

void InitDynamicMem();
unsigned char Inb(unsigned short port);
void Outb(unsigned short port, unsigned char data);
void Outw(unsigned short port, unsigned short value);
void Delay(uint16_t Ms);
void *MemoryCopy(unsigned char *dst, unsigned char *src, int n);
void* MemorySet(void * ptr, int value, uint16_t num);
int MemoryCompare(const void* ptr1, const void* ptr2, uint16_t num);
void *MemoryAlloc(size_t size);
void MemoryFree(void *p);
const char* StringChar(const char* String, char Char);
char* StringTok(char* String, const char* Delim);
char* StringCopy(char* destination, const char* source);
size_t StringCspn(const char* S, const char* Reject);
size_t StringSpn(const char* S, const char* Accept);
uint32_t StringLength(const char *String);
bool Backspace(char Buffer[]);
void Append(char s[], char n);
void InitPic(void);
char Toupper(char chr);