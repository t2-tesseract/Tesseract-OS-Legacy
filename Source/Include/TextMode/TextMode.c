#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
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

void TerminalBack(){
	TerminalCol -= 1;
	TerminalPutChar(' ');
	TerminalCol -= 1;
}

void SetCharAtVideoMemory(char Character, int Offset){
    unsigned char *VideoMemory = (unsigned char *) 0xb8000;
    VideoMemory[Offset] = Character;
    VideoMemory[Offset + 1] = 0x0F;
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

int GetCursor(){
    Outb(VgaCtrlRegister, VgaOffsetHigh);
    int Offset = Inb(VgaDataRegister) << 8;
    Outb(VgaCtrlRegister, VgaOffsetLow);
    Offset += Inb(VgaDataRegister);
    return Offset * 2;
}

int CompareString(char String1[], char String2[]) {
    int i;
    for (i = 0; String1[i] == String2[i]; i++) {
        if (String1[i] == '\0') return 0;
    }
    return String1[i] - String2[i];
}

void ExecuteCommand(char *Input){
    if (CompareString(Input, "shutdown") == 0) {
        TerminalWrite("Stopping the CPU. Bye!\n");
        asm volatile("hlt");
    }
	TerminalSetColor(0x0C);
    TerminalWrite("Unknown command\n");
    TerminalSetColor(0x0B);
	TerminalWrite("Tesseract$ ");
	TerminalSetColor(0x0F);
}
