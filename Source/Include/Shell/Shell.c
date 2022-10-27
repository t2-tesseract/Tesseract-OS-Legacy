#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/TextMode/TextMode.h>
#include <Include/Vfs/Vfs.h>
#include "Shell.h"

char* Argv;

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

		TerminalWrite("\n");
	} else if (CompareString(Input, "mk") == 0) {
		Files = CreateFile(Arg1, "", 2, Files);
		TerminalWrite("\n\n");
	} else if (CompareString(Input, "cd") == 0) {
		// todo: make it so that its compatible with created folders
		if (CompareString(Arg1, "usr") == 0) {
			Folders = ChangeDirectory(Folders, "usr");
		} else if (CompareString(Arg1, "bin") == 0) {
			Folders = ChangeDirectory(Folders, "bin");
		} else if (CompareString(Arg1, "dev") == 0) {
			Folders = ChangeDirectory(Folders, "dev");
		} else if (CompareString(Arg1, "home") == 0) {
			Folders = ChangeDirectory(Folders, "home");
		} else if (CompareString(Arg1, "lib") == 0) {
			Folders = ChangeDirectory(Folders, "lib");
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
		TerminalWrite(Tab);

		TerminalWrite("\"");
		TerminalWrite(Arg1);
		TerminalWrite("\"");

		TerminalWrite("\n\n");
	} else if (CompareString(Input, "chlan") == 0) {
		TerminalSetColor(0x0E);
		TerminalWrite(Tab);

		if (CompareString(Arg1, "english") == 0 || CompareString(Arg1, "en") == 0) {
			Language = "english";

			TerminalWrite("System has successfully been changed to English.");
			TerminalWrite("\n\n");
		} else if (CompareString(Arg1, "french") == 0 || CompareString(Arg1, "fr") == 0) {
			Language = "french";

			TerminalWrite("Le système a bien été changé en Français.");
			TerminalWrite("\n\n");
		} else if (CompareString(Arg1, "tessong") == 0 || CompareString(Arg1, "te") == 0) {
			Language = "tessong";

			TerminalWrite("Ýma System chänge hassê bin Tessong ïn.");
			TerminalWrite("\n\n");
		} else {
			if (Language = "french") {
				TerminalWrite("Cette langue n'existe pas.");
			} else if (Language = "tessong") {
				TerminalWrite("Thëss lungaë exïtt nôn.");
			} else {
				TerminalWrite("This language does not exist.");
			}
		}
	} else {
		TerminalSetColor(0x0C);

		TerminalWrite(Tab);
		TerminalWrite("Unknown command, make sure it is written correctly.");
		TerminalWrite("\n\n");
	}

	TerminalShell();
}

