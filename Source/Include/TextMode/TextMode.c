#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/Graphics/Graphics.h>
#include <Include/Vfs/Vfs.h>
#include "Printk.h"
#include "TextMode.h"

char* Argv;

volatile uint16_t* VgaBuffer = (uint16_t*)0xB8000;

const int VgaCols = 80;
const int VgaRows = 25;

int TerminalCol = 0;
int TerminalRow = 0;
uint8_t TerminalColor = 0x0F;

const char* curDir = "";

const char* Resolution = "80x25";

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
	} else {
		//log 
		TerminalSetColor(0x08);
		TerminalWrite("Log: ");

		TerminalSetColor(0x0F);
		TerminalWrite(String);
	}

	/*if (toEmuConsole) {
		while (*String) {
			i686_outb(0xE9, *String);
			String++;
		} 
	}*/
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

void ExecuteCommand(char *Input, char *Arg1){
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
	} else if (CompareString(Input, "restart") == 0) {
		TerminalClear();

		TerminalSetColor(0x0C);

		// TerminalWrite(Tab);
		TerminalWrite("Restarting...");

		Delay(2500);

		uint8_t good = 0x02;
		while (good & 0x02)
			good = Inb(0x64);
		Outb(0x64, 0xFE);
		asm("hlt");
    } else if (CompareString(Input, "help") == 0) {
		// general commands
		char *generalCommandsList[] = {
			"shutdown", 
			"clr",
			"rpt"
		};
		char *generalDescList[] = {
			"Halt the CPU and shutdown the computer.",
			"Clear the screen.",
			"Repeat a string."
		};
		size_t generalArraySize = sizeof(generalCommandsList) / sizeof(generalCommandsList[0]);

		// settings
		char *settingsCommandsList[] = {
			"lv",
			"sv"
		};
		char *settingsDescList[] = {
			"List video modes.",
			"Set a video mode"
		};
		size_t settingsArraySize = sizeof(settingsCommandsList) / sizeof(settingsCommandsList[0]);

		// file system
		char *fsCommandsList[] = {
			"ls",
			"cd"
		};
		char *fsDescList[] = {
			"List the files and folders in the current directory.",
			"Change the current directory."
		};
		size_t fsArraySize = sizeof(fsCommandsList) / sizeof(fsCommandsList[0]);

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

		while(a != (int)generalArraySize) {
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

		while(a != (int)settingsArraySize) {
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
		TerminalWrite("File System: \n");

		a = 0;

		while(a != (int)fsArraySize) {
			int e;

			for (e = 0; e < 3; e++) {
				TerminalWrite(Tab);
			}

			TerminalSetColor(0x0C);
			TerminalWrite(fsCommandsList[a]);
			TerminalWrite(" ");
			
			TerminalSetColor(0x0F);
			TerminalWrite("- ");
			TerminalWrite(fsDescList[a]);
			TerminalWrite("\n");

			a++;
		};

		TerminalSetColor(0x02);

		for (i = 0; i < 2; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("Miscellaneous: \n");

		a = 0;

		while(a != (int)miscArraySize) {
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

		TerminalSetColor(0x07);

		for (i = 0; i < 2; i++) {
			TerminalWrite(Tab);
		}
		TerminalWrite("For arguments, you have to put \":\" next to the command:\n");

		for (i = 0; i < 3; i++) {
			TerminalWrite(Tab);
		}
		TerminalSetColor(0x0A);
		TerminalWrite("Example: cd:usr");

		TerminalWrite("\n\n");
    } else if (CompareString(Input, "lv") == 0) {
		char *res[] = {"80x25"};
		size_t arraySize = sizeof(res) / sizeof(res[0]);
		int a = 0;

		TerminalSetColor(0x0A);

		TerminalWrite(Tab);
		TerminalWrite("Resolutions available:\n");

		TerminalSetColor(0x0E);

		while(a != (int)arraySize) {
			for (i = 0; i < 2; i++) {
				TerminalWrite(Tab);
			}

			TerminalWrite("- ");
			TerminalWrite(res[a]);
			TerminalWrite("\n");

			a++;
		}
		// TerminalWrite("2: 320x200 VGA graphic mode\n");

		TerminalWrite("\n\n");
	} else if (CompareString(Input, "sv") == 0) {
		// finally
		if (CompareString(Arg1, "80x25") == 0) {
			Resolution = "80x25";
		} else if (CompareString(Arg1, "reset") == 0) {
			Resolution = "80x25";
		}
		TerminalWrite("\n\n");
	} else if (CompareString(Input, "clr") == 0) {
		TerminalClear();
	} else if (CompareString(Input, "ls") == 0) {
		// struct File* Read = CreateFile("Readme.txt", "Welcome to Tesseract!\nWARNING, This project is at it's earliest stage, there might be bugs or missing functionalities.\n\nTesseract is a project made by T2 (alias iplux and Kokolor) destined to make an working operating system from scratch.\nWhy a new operating system? Why not choose a GNU/Linux distro, Windows or macOS?\nBecause we believe Tesseract is not like the others. Tesseract can change what you think of operating systems.\n\nA tesseract is basically a 4D shape, which describes Tesseract. An operating system from a new dimension.", StringLength("Welcome to Tesseract!\nWARNING, This project is at it's earliest stage, there might be bugs or missing functionalities.\n\nTesseract is a project made by T2 (alias iplux and Kokolor) destined to make an working operating system from scratch.\nWhy a new operating system? Why not choose a GNU/Linux distro, Windows or macOS?\nBecause we believe Tesseract is not like the others. Tesseract can change what you think of operating systems.\n\nA tesseract is basically a 4D shape, which describes Tesseract. An operating system from a new dimension."), Files);
    	// struct File* Test = CreateFile("Test.txt", "This is a test", StringLength("This is a test"), Files);
		ListFolders(Folders);
		ListFiles(Files);
	} else if (CompareString(Input, "create") == 0) {
		Files = CreateFile(Arg1, "", 2, Files);
	} else if (CompareString(Input, "cd") == 0) {
		// todo: make it so that its compatible with created folders
		if (CompareString(Arg1, "usr") == 0) {
			Folders = GoToFolder(Folders, "usr");
			curDir = "usr";
		} else if (CompareString(Arg1, "bin") == 0) {
			Folders = GoToFolder(Folders, "bin");
			curDir = "bin";
		} else if (CompareString(Arg1, "dev") == 0) {
			Folders = GoToFolder(Folders, "dev");
			curDir = "dev";
		} else if (CompareString(Arg1, "home") == 0) {
			Folders = GoToFolder(Folders, "home");
			curDir = "home";
		} else if (CompareString(Arg1, "lib") == 0) {
			Folders = GoToFolder(Folders, "lib");
			curDir = "lib";
		} else if (CompareString(Arg1, "") == 0) {
			TerminalSetColor(0x0C);
			TerminalWrite(Tab);
			TerminalWrite("Please specify a folder.\n");
		} else {
			TerminalSetColor(0x0C);
			TerminalWrite(Tab);
			TerminalWrite("The folder specified does not exist.\n");
		}

		TerminalWrite("\n");
	/*} else if (CompareString(Input, "cd usr") == 0) {
		Folders = GoToFolder(Folders, "usr");
		curDir = "usr";
	} else if (CompareString(Input, "cd bin") == 0) {
		Folders = GoToFolder(Folders, "bin");
		curDir = "bin";
	} else if (CompareString(Input, "cd dev") == 0) {
		Folders = GoToFolder(Folders, "dev");
		curDir = "dev";
	} else if (CompareString(Input, "cd home") == 0) {
		Folders = GoToFolder(Folders, "home");
		curDir = "home";
	} else if (CompareString(Input, "cd lib") == 0) {
		Folders = GoToFolder(Folders, "lib");
		curDir = "lib";*/
	} else if (CompareString(Input, "sysfetch") == 0) {
		TerminalWrite("\n");

		TerminalSetColor(0x0B);
		for (i = 0; i < 34; i++) {
			TerminalWrite(Tab);
			TerminalWrite(logo[i]);
		}

		TerminalWrite("\n\n");
	} else if (CompareString(Input, "rpt") == 0) {
		TerminalSetColor(0x0A);
		TerminalWrite(Arg1);
		TerminalWrite("\n\n");
	} else {
		TerminalSetColor(0x0C);

		TerminalWrite(Tab);
		TerminalWrite("Unknown command, make sure it is written correctly.");
		TerminalWrite("\n\n");
	}

	TerminalShell();
}

