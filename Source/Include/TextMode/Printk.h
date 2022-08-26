#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "TextMode.h"

void Itoa(char *buf, unsigned long int n, int base);
void PutChar(char c);
void Printk(char* Format, ...);