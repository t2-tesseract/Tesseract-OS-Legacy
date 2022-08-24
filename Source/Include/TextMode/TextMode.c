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

void TerminalClear(bool ResetPos) {
	for (int col = 0; col < VgaCols; col ++)
	{
		for (int row = 0; row < VgaRows; row ++)
		{
			const size_t index = (VgaCols * row) + col;
			VgaBuffer[index] = ((uint16_t)TerminalColor << 8) | ' ';
		}
	}

	if (ResetPos) {
		TerminalCol = 0;
		TerminalRow = 0;
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

void TerminalShell() {
	Outb(0x3D4, 0x0A);
	Outb(0x3D5, 0x20);
	TerminalSetColor(0x0B);
	TerminalWrite("Tesseract ");

	TerminalSetColor(0x02);
	TerminalWrite("[/]");

	TerminalSetColor(0x0F);
	TerminalWrite("$ ");
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
		// shutdown and clear the screen to show the message
		TerminalClear(true);
		TerminalSetColor(0x0C);
		TerminalWrite("Stopping the CPU.\n\n");

		TerminalSetColor(0x0A);
		TerminalWrite("It is now safe to turn off the computer.");
		asm volatile("hlt");
    }
	else if (CompareString(Input, "help") == 0){
		TerminalSetColor(0x0F);
		TerminalWrite("shutdown: Halt the CPU\n");
		TerminalShell();
	}
	else{
		TerminalSetColor(0x0C);
		TerminalWrite("Unknown command\n");
		TerminalShell();
	}
}
