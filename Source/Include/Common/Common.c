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

char* StringNCat(char * String1, char * String2, uint32_t n){
    char * S = String1;
    String1 += StringLength(String1);
    uint32_t SS = __StringLength(String2,n);
    String1[SS] = '\0';
    MemoryCopy(String1, String2,SS);
    return S;
}

// uint32_t StringLength(const char *String) {
//     int i = 0;
//     while (String[i] != (char)0) {
//         ++i;
//     }
//     return i;
// }

uint32_t __StringLength(const char *String, uint32_t MaxLen) {
    int i = 0;
    while (String[i] != (char)0) {
        ++i;
        if (i==MaxLen) return i;
    }
    return i;
}

size_t StringLength(const char* String){
    size_t Len = 0;
    while(String[Len] != '\0')
        Len++;
    return Len;
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
