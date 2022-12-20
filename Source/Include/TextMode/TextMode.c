#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Graphics/Graphics.h>
#include <Include/Vfs/Vfs.h>
#include "Printk.h"
#include "TextMode.h"

volatile uint16_t* VgaBuffer = (uint16_t*)0xB8000;

const int VgaCols = 80;
const int VgaRows = 25;

int TerminalCol = 0;
int TerminalRow = 0;
uint8_t TerminalColor = 0x0F;

char* logo[34] = {
			"              &&&             \n",
			"             &&&&&            \n",
			"            &&&&&&&           \n",
			"          (&&&&&&&&&#         OS: Tesseract Operating System\n",
			"         &&&&&&&&&&&&&        \n",
			"        &&&&&&&&&&&&&&&       Version: 1.0\n",
			"       &&&&&&&&&&&&&&&&&      Compile Date: todo\n",
			"      &&&&&&&&&&&&&&&&&&&     \n",
			"         %%%%%%%%%%%%%        \n",
			"      %%%%%%%%%%%%%%%%%%%     \n",
			"   %%%%%%%%%%%%%%%%%%%%%%%%%  \n",
			"  %%%%%%%%%%%%%%%%%%%%%%%%%%% \n",
			"    %%%%%%%%%%%%%%%%%%%%%%%   \n",
			"       %%%%%%%%%%%%%%%%%      \n",
			"         (%%%%%%%%%%%(        \n",
			"        (((((%%%%%(((((       Made by: T2\n",
			"       (((((((((((((((((      (iplux and Kokolor)\n",
			"      (((((((((((((((((((     \n",
			"         (((((((((((((        \n",
			"            (((((((           "
		};

const char* Tab = "    ";

void SetCursor(int Offset){
    Offset /= 2;
    Outb(VgaCtrlRegister, VgaOffsetHigh);
    Outb(VgaDataRegister, (unsigned char) (Offset >> 8));
    Outb(VgaCtrlRegister, VgaOffsetLow);
    Outb(VgaDataRegister, (unsigned char) (Offset & 0xff));
}

int GetCursor(){
    Outb(VgaCtrlRegister, VgaOffsetHigh);
    int Offset = Inb(VgaDataRegister) << 8;
    Outb(VgaCtrlRegister, VgaOffsetLow);
    Offset += Inb(VgaDataRegister);
    return Offset * 2;
}

int GetOffset(int Col, int Row){
    return 2 * (Row * 80 + Col);
}

int GetRowFromOffset(int Offset){
    return Offset / (2 * 80);
}

int MoveOffsetToNewLine(int Offset){
    return GetOffset(0, GetRowFromOffset(Offset) + 1);
}

void TerminalPutChar(char Character, int Offset){
    unsigned char *VideoMemory = (unsigned char *) 0xb8000;
    VideoMemory[Offset] = Character;
    VideoMemory[Offset + 1] = TerminalColor;
}

int Scroll(int Offset){
    MemoryCopy(
             (uint8_t *) (GetOffset(0, 0) + 0xb8000),
             (uint8_t *) (GetOffset(0, 1) + 0xb8000),
             VgaCols * (VgaRows - 1) * 2
    );

    for (int Col = 0; Col < VgaCols; Col++) {
        TerminalPutChar(' ', GetOffset(Col, VgaRows - 1));
    }
	
    return Offset - 2 * VgaCols;
}

void TerminalWrite(const char* String){
    int Offset = GetCursor();
    int i = 0;
    while (String[i] != 0) {
        if (Offset >= VgaRows * VgaCols * 2) {
            Offset = Scroll(Offset);
			Delay(10);
			//TerminalClear();
        }
		
        if (String[i] == '\n') {
            Offset = MoveOffsetToNewLine(Offset);
        } else {
            TerminalPutChar(String[i], Offset);
            Offset += 2;
        }

        i++;
    }

    SetCursor(Offset);
}

void TerminalClear(){
    for (int i = 0; i < 80 * 25; ++i) {
        TerminalPutChar(' ', i * 2);
    }
    SetCursor(GetOffset(0, 0));
	SetCursor(0);
}

void TerminalSetColor(uint8_t Color){
    TerminalColor = Color;
}

void TerminalBack(){
	int NewCursor = GetCursor() - 2;
    TerminalPutChar(' ', NewCursor);
    SetCursor(NewCursor);
}

void DebugWrite(const char* String, int Mode){
	if (Mode == 0) {
		//kernel
		TerminalSetColor(0x02);
		TerminalWrite("Kernel: ");

		TerminalSetColor(0x0F);
		TerminalWrite(String);
	} else if (Mode == 1) {
		//warning
		TerminalSetColor(0x0E);
		TerminalWrite("Warning: ");

		TerminalSetColor(0x0F);
		TerminalWrite(String);
	} else if (Mode == 2) {
		//error
		TerminalSetColor(0x04);
		TerminalWrite("Error: ");

		TerminalSetColor(0x0F);
		TerminalWrite(String);
	} else if (Mode == 3) {
		//success
		TerminalSetColor(0x0A);
		TerminalWrite("Success: ");

		TerminalSetColor(0x0F);
		TerminalWrite(String);
	} else {
		//log 
		TerminalSetColor(0x08);
		TerminalWrite("Log: ");

		TerminalSetColor(0x0F);
		TerminalWrite(String);
	}

	while (*String) {
		Outb(0xE9, *String);
		String++;
	} 
}

void TerminalShell(){
	TerminalSetColor(0x0B);
	TerminalWrite("Tesseract ");

	TerminalSetColor(0x02);
	TerminalWrite("[");

	TerminalWrite(curDir);

	TerminalWrite("/] ");

	TerminalSetColor(0x0F);
	TerminalWrite("$> ");
}

int CompareString(char String1[], char String2[]) {
    int i;
    for (i = 0; String1[i] == String2[i]; i++) {
        if (String1[i] == '\0') return 0;
    }
    return String1[i] - String2[i];
}

