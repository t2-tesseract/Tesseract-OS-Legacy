#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include "TextMode.h"

volatile uint16_t* VgaBuffer = (uint16_t*)0xB8000;

const int VgaCols = 80;
const int VgaRows = 25;

int TerminalCol = 0;
int TerminalRow = 0;
uint8_t TerminalColor = 0x0F;

void TerminalClear(){
	for (int col = 0; col < VgaCols; col ++)
	{
		for (int row = 0; row < VgaRows; row ++)
		{
			const size_t index = (VgaCols * row) + col;
			VgaBuffer[index] = ((uint16_t)TerminalColor << 8) | ' ';
		}
	}
}

void TerminalSetColor(uint8_t Color){
    TerminalColor = Color;
}

void TerminalPutChar(char c){
	switch (c){
	case '\n':
		{
			TerminalCol = 0;
			TerminalRow ++;
			break;
		}
 
	default:
		{
			const size_t index = (VgaCols * TerminalRow) + TerminalCol;
			VgaBuffer[index] = ((uint16_t)TerminalColor << 8) | c;
			TerminalCol ++;
			break;
		}
	}

	if (TerminalCol >= VgaCols)
	{
		TerminalCol = 0;
		TerminalRow ++;
	}

	if (TerminalRow >= VgaRows)
	{
		TerminalCol = 0;
		TerminalRow = 0;
	}
}

void TerminalWrite(const char* String){
	for (size_t i = 0; String[i] != '\0'; i ++)
		TerminalPutChar(String[i]);
}