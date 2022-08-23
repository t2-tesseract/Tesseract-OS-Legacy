#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>

void *MemoryCopy(char *dst, char *src, int n){
	char *p = dst;
	while (n--)
		*dst++ = *src++;
	return p;
}