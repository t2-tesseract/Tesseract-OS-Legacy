#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "TextMode.h"

void Itoa(char *buf, unsigned long int n, int base);
void PrintkSetColor(uint8_t SetColor);
void PutChar(char c);
void Printk(char* Format, ...);