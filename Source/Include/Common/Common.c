#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Common.h"

void *MemoryCopy(char *dst, char *src, int n){
	char *p = dst;
	while (n--)
		*dst++ = *src++;
	return p;
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