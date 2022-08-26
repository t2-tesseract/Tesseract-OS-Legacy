#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include "Printk.h"

volatile uint16_t* Buffer = (uint16_t*)0xB8000;

const int Cols = 80;
const int Rows = 25;

int Col = 0;
int Row = 0;
uint8_t Color = 0x0F;

void Itoa(char *buf, unsigned long int n, int base){
	unsigned long int tmp;
	int i, j;

	tmp = n;
	i = 0;

	do {
		tmp = n % base;
		buf[i++] = (tmp < 10) ? (tmp + '0') : (tmp + 'a' - 10);
	} while (n /= base);
	buf[i--] = 0;

	for (j = 0; j < i; j++, i--) {
		tmp = buf[j];
		buf[j] = buf[i];
		buf[i] = tmp;
	}
}

void PutChar(char c){
	switch (c)
	{
	case '\n':
		{
			Col = 0;
			Row ++;
			break;
		}
 
	default:
		{
			const size_t Index = (Cols * Row) + Col;
			Buffer[Index] = ((uint16_t)Color << 8) | c;
			Col ++;
			break;
		}
	}

	if (Col >= Cols)
	{
		Col = 0;
		Row ++;
	}

	if (Row >= Rows)
	{
		Col = 0;
		Row = 0;
	}
}

void Printk(char* Format, ...){
    va_list ap;

	char buf[16];
	int i, j, size, buflen, neg;

	unsigned char c;
	int ival;
	unsigned int uival;

	va_start(ap, Format);

	while ((c = *Format++)) {
		size = 0;
		neg = 0;

		if (c == 0)
			break;
		else if (c == '%') {
			c = *Format++;
			if (c >= '0' && c <= '9') {
				size = c - '0';
				c = *Format++;
			}

			if (c == 'd') {
				ival = va_arg(ap, int);
				if (ival < 0) {
					uival = 0 - ival;
					neg++;
				} else
					uival = ival;
				Itoa(buf, uival, 10);

				buflen = StringLength(buf);
				if (buflen < size)
					for (i = size, j = buflen; i >= 0;
					     i--, j--)
						buf[i] =
						    (j >=
						     0) ? buf[j] : '0';

				if (neg)
					Printk("-%s", buf);
				else
					Printk(buf);
			} else if (c == 'u') {
				uival = va_arg(ap, int);
				Itoa(buf, uival, 10);

				buflen = StringLength(buf);
				if (buflen < size)
					for (i = size, j = buflen; i >= 0;
					     i--, j--)
						buf[i] =
						    (j >=
						     0) ? buf[j] : '0';

				Printk(buf);
			} else if (c == 'x' || c == 'X') {
				uival = va_arg(ap, int);
				Itoa(buf, uival, 16);

				buflen = StringLength(buf);
				if (buflen < size)
					for (i = size, j = buflen; i >= 0;
					     i--, j--)
						buf[i] =
						    (j >=
						     0) ? buf[j] : '0';

				Printk("0x%s", buf);
			} else if (c == 'p') {
				uival = va_arg(ap, int);
				Itoa(buf, uival, 16);
				size = 8;

				buflen = StringLength(buf);
				if (buflen < size)
					for (i = size, j = buflen; i >= 0;
					     i--, j--)
						buf[i] =
						    (j >=
						     0) ? buf[j] : '0';

				Printk("0x%s", buf);
			} else if (c == 's') {
				Printk((char *) va_arg(ap, int));
			} 
		} else
			PutChar(c);
	}

	return;
}

