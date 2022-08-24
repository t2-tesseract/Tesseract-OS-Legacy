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
	TerminalWrite("[usr/] ");

	TerminalSetColor(0x0F);
	TerminalWrite("$> ");
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
	int i;

	// TerminalWrite("\n");

    if (CompareString(Input, "shutdown") == 0) {
		// shutdown and clear the screen to show the message
		TerminalSetColor(0x0C);

		TerminalWrite(Tab);
		TerminalWrite("Stopping the CPU.");

		Delay(2500);

		TerminalClear(true);

		Delay(1250);

		TerminalSetColor(0x0A);
		TerminalWrite("It is now safe to turn off the computer.");
		asm volatile("hlt");
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
			TerminalWrite("\n");

			a++;
		};

		/*for (i = 0; i < 4; i++) {
			int a;

			for (a = 0; a < 3; a++) {
				TerminalWrite(Tab);
			}
			TerminalWrite(commandsList[i]);
		}*/

		/*TerminalSetColor(0x0C);

		for (i = 0; i < 3; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("shutdown ");

		TerminalSetColor(0x0F);
		TerminalWrite("- Halt the CPU and shutdown the computer.\n");

		TerminalSetColor(0x0C);

		for (i = 0; i < 3; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("lsv ");

		TerminalSetColor(0x0F);
		TerminalWrite("- List and set video modes.\n");

		TerminalSetColor(0x0C);

		for (i = 0; i < 3; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("clr ");

		TerminalSetColor(0x0F);
		TerminalWrite("- Clear the screen.\n");

		TerminalSetColor(0x0C);

		for (i = 0; i < 3; i++) {
			TerminalWrite(Tab);
		}

		TerminalWrite("sysfetch ");

		TerminalSetColor(0x0F);
		TerminalWrite("- Get system information.");*/


		TerminalWrite("\n\n");
    } else if (CompareString(Input, "lsv") == 0) {
		TerminalSetColor(0x0E);

		TerminalWrite(Tab);
		TerminalWrite("1: 80x25 Text mode");
		// TerminalWrite("2: 320x200 VGA graphic mode\n");

		// TODO: make a mini shell to select resolution number (ex "1" for 80x25 text mode)

		TerminalWrite("\n\n");
	} else if (CompareString(Input, "clr") == 0) {
		TerminalClear(true);
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
