#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Graphics/Graphics.h>
#include "TextMode.h"

volatile uint16_t* VgaBuffer = (uint16_t*)0xB8000;

const int VgaCols = 80;
const int VgaRows = 25;

int TerminalCol = 0;
int TerminalRow = 0;
uint8_t TerminalColor = 0x0F;

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
            (char *) (GetOffset(0, 1) + 0xb8000),
            (char *) (GetOffset(0, 0) + 0xb8000),
            80 * (25 - 1) * 2
    );

    for (int Col = 0; Col < 80; Col++) {
        TerminalPutChar(' ', GetOffset(Col, 80 - 1));
    }

    return Offset - 2 * 80;
}

void TerminalWrite(const char* String){
    int Offset = GetCursor();
    int i = 0;
    while (String[i] != 0) {
        if (Offset >= 25 * 80 * 2) {
            Offset = Scroll(Offset);
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
	} else {
		//log 
		TerminalSetColor(0x08);
		TerminalWrite("Log: ");

		TerminalSetColor(0x0F);
		TerminalWrite(String);
	}
}

void TerminalShell(){
	TerminalSetColor(0x0B);
	TerminalWrite("Tesseract ");

	TerminalSetColor(0x02);
	TerminalWrite("[usr/] ");

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

void ExecuteCommand(char *Input){
	int i;

    if (CompareString(Input, "shutdown") == 0) {
		// shutdown and clear the screen to show the message
		TerminalClear();

		TerminalSetColor(0x0C);

		// TerminalWrite(Tab);
		TerminalWrite("Stopping the CPU.\nSee you soon!");

		Delay(2500);

		Outw(0xB004, 0x2000);
		Outw(0x604, 0x2000);
		Outw(0x4004, 0x3400);
		asm("hlt");
    } else if (CompareString(Input, "help") == 0) {
		// general commands
		char *generalCommandsList[] = {
			"shutdown", 
			"clr"
		};
		char *generalDescList[] = {
			"Halt the CPU and shutdown the computer.",
			"Clear the screen."
		};
		size_t generalArraySize = sizeof(generalCommandsList) / sizeof(generalCommandsList[0]);

		// settings
		char *settingsCommandsList[] = {
			"lsv"
		};
		char *settingsDescList[] = {
			"List and set video modes."
		};
		size_t settingsArraySize = sizeof(settingsCommandsList) / sizeof(settingsCommandsList[0]);

		// miscellaneous
		char *miscCommandsList[] = {
			"sysfetch"
		};
		char *miscDescList[] = {
			"Get system information."
		};
		size_t miscArraySize = sizeof(miscCommandsList) / sizeof(miscCommandsList[0]);

		TerminalSetColor(0x0A);

		TerminalWrite(Tab);
		TerminalWrite("Tesseract Help: \n");

		TerminalSetColor(0x02);

		for (i = 0; i < 2; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("General: \n");

		int a = 0;

		while(a != generalArraySize) {
			int e;

			for (e = 0; e < 3; e++) {
				TerminalWrite(Tab);
			}

			TerminalSetColor(0x0C);
			TerminalWrite(generalCommandsList[a]);
			TerminalWrite(" ");
			
			TerminalSetColor(0x0F);
			TerminalWrite("- ");
			TerminalWrite(generalDescList[a]);
			TerminalWrite("\n");

			a++;
		};

		TerminalSetColor(0x02);

		for (i = 0; i < 2; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("Settings: \n");

		a = 0;

		while(a != settingsArraySize) {
			int e;

			for (e = 0; e < 3; e++) {
				TerminalWrite(Tab);
			}

			TerminalSetColor(0x0C);
			TerminalWrite(settingsCommandsList[a]);
			TerminalWrite(" ");
			
			TerminalSetColor(0x0F);
			TerminalWrite("- ");
			TerminalWrite(settingsDescList[a]);
			TerminalWrite("\n");

			a++;
		};

		TerminalSetColor(0x02);

		for (i = 0; i < 2; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("Miscellaneous: \n");

		a = 0;

		while(a != miscArraySize) {
			int e;

			for (e = 0; e < 3; e++) {
				TerminalWrite(Tab);
			}

			TerminalSetColor(0x0C);
			TerminalWrite(miscCommandsList[a]);
			TerminalWrite(" ");
			
			TerminalSetColor(0x0F);
			TerminalWrite("- ");
			TerminalWrite(miscDescList[a]);
			TerminalWrite("");

			a++;
		};


		TerminalWrite("\n\n");
    } else if (CompareString(Input, "lsv") == 0) {
		TerminalSetColor(0x0E);

		TerminalWrite(Tab);
		TerminalWrite("1: 80x25 Text mode");
		// TerminalWrite("2: 320x200 VGA graphic mode\n");

		// TODO: make a mini shell to select resolution number (ex "1" for 80x25 text mode)

		TerminalWrite("\n\n");
	} else if (CompareString(Input, "clr") == 0) {
		TerminalClear();
	} else if (CompareString(Input, "sysfetch") == 0) {
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

		TerminalWrite("\n");

		TerminalSetColor(0x0B);
		for (i = 0; i < 34; i++) {
			TerminalWrite(Tab);
			TerminalWrite(logo[i]);
		}

		TerminalWrite("\n");
	} else {
		TerminalSetColor(0x0C);

		TerminalWrite(Tab);
		TerminalWrite("Unknown command, make sure it is written correctly.");
		TerminalWrite("\n\n");
	}

	TerminalShell();
}