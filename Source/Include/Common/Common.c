#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Common.h"

unsigned char Inb(unsigned short port) {
    unsigned char result;
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void Outb(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

void Outw(unsigned short port, unsigned short value){
    asm volatile ("outw %%ax,%%dx": :"dN"(port), "a"(value));
} 

static inline void IoWait(void){
    Outb(0x80, 0);
}

void Delay(uint16_t Ms){
    for(long long int i=0; i<5000*(uint16_t)Ms / 2; i++)
        IoWait();
}

void *MemoryCopy(char *dst, char *src, int n){
	int i;
    for (i = 0; i < n; i++) {
        *(dst + i) = *(src + i);
    }
}

void* MemorySet(void * ptr, int value, uint16_t num){
    uint8_t* u8Ptr = (uint8_t *)ptr;

    for (uint16_t i = 0; i < num; i++)
        u8Ptr[i] = (uint8_t)value;

    return ptr;
}

int MemoryCompare(const void* ptr1, const void* ptr2, uint16_t num){
    const uint8_t* u8Ptr1 = (const uint8_t *)ptr1;
    const uint8_t* u8Ptr2 = (const uint8_t *)ptr2;

    for (uint16_t i = 0; i < num; i++)
        if (u8Ptr1[i] != u8Ptr2[i])
            return 1;

    return 0;
}

const char* StringChar(const char* str, char chr){
    if (str == NULL)
        return NULL;

    while (*str)
    {
        if (*str == chr)
            return str;

        ++str;
    }

    return NULL;
}

uint32_t StringLength(const char *String){
    int i = 0;
    while (String[i] != (char)0) {
        ++i;
    }
    return i;
}

bool Backspace(char Buffer[]){
    int Len = StringLength(Buffer);
    if (Len > 0) {
        Buffer[Len - 1] = '\0';
        return true;
    } else {
        return false;
    }
}

void Append(char s[], char n){
    int Len = StringLength(s);
    s[Len] = n;
    s[Len + 1] = '\0';
}

void InitPic(void){
	Outb(0x20, 0x11);
	Outb(0xA0, 0x11);

	Outb(0x21, 0x20);
	Outb(0xA1, 0x70);

	Outb(0x21, 0x04);
	Outb(0xA1, 0x02);

	Outb(0x21, 0x01);
	Outb(0xA1, 0x01);

	Outb(0x21, 0x0);
	Outb(0xA1, 0x0);
}

bool IsLower(char chr){
    return chr >= 'a' && chr <= 'z';
}

char Toupper(char chr){
    return IsLower(chr) ? (chr - 'a' + 'A') : chr;
}
