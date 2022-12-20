#include <stddef.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include <Include/TextMode/TextMode.h>
#include <Include/Vfs/Vfs.h>
#include "Shell.h"

char* Argv;

struct UserVar *userVars = NULL;

void ExecuteCommand(char *Input, char *Arg1, char *Arg2){
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
			"cd",
			"mkf",
			"mkd"
		};
		char *fsDescList[] = {
			"List the files and folders in the current directory.",
			"Change the current directory.",
			"Create a file with the provided name and data.",
			"Create a folder with the provided name."
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
	} else if (CompareString(Input, "mkf") == 0) {
		Files = CreateFile(Arg1, Arg2, 2, Files);

		TerminalWrite("Successfully created the file: \"");

		//TerminalSetColor();
		TerminalWrite(Arg1);

		TerminalWrite("\"\ncontaining the data: \"");
		TerminalWrite(Arg2);

		TerminalWrite("\".\n\n");
	} else if (CompareString(Input, "mkd") == 0) {
		Folders = CreateFolder(Arg1);

		DebugWrite("Successfully created the folder: \"", 3);

		//TerminalSetColor();
		TerminalWrite(Arg1);
		
		TerminalWrite("\".\n\n");
	} else if (CompareString(Input, "cd") == 0) {
		// Made so that it's compatible with user-created folders, part of the code made by AI (ChatGPT).
		#define MAX_FOLDERS 999
		struct Folder folderArray[MAX_FOLDERS];

		struct Folder* current = Folders;
		int i = 0;
		while (current != NULL && i < MAX_FOLDERS) {
			folderArray[i++] = *current;
			current = current->Next;
		}

		bool folderFound = false;
		size_t folderArraySize = sizeof(folderArray) / sizeof(folderArray[0]);

		for (int i = 0; i < folderArraySize; i++) {
  			if (CompareString(Arg1, folderArray[i].Name) == 0) {
				Folders = ChangeDirectory(Folders, Arg1);
				folderFound = true;
				break;
			}
		}

		if (!folderFound) {
			TerminalSetColor(0x0C);
			TerminalWrite(Tab);
			TerminalWrite("The folder specified does not exist.\n");
		}

		TerminalWrite("\n");
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
	} else if (CompareString(Input, "insv") == 0) {
		struct UserVar *uv = MemoryAlloc(sizeof(struct UserVar));
		if (uv == NULL) {
			// Error: unable to allocate memory for the new variable
			TerminalWrite("not work\n\n");
		}

		uv->Name = Arg1;
		uv->Value = Arg2;
		uv->Next = userVars;

		userVars = uv;

		TerminalWrite("work whoohoo\n\n");
		TerminalWrite(userVars);
	} else if (CompareString(Input, "listv") == 0) {
		#define MAX_VARIABLES 999
		struct UserVar userVarArray[MAX_VARIABLES];

		struct UserVar* current = userVars;
		int i = 0;
		while (current != NULL && i < MAX_VARIABLES) {
			userVarArray[i++] = *current;
			current = current->Next;
		}

		size_t userVarArraySize = sizeof(userVarArray) / sizeof(userVarArray[0]);

		TerminalWrite(userVarArray->Name);
		TerminalWrite("\n");
		
		/*struct UserVar *uv = usVar;
		
		while (uv != NULL) {
			TerminalWrite(Tab);
			TerminalWrite(uv->Name);
			TerminalWrite("\n");
			uv = uv->Next;
		}*/
	} else {
		TerminalSetColor(0x0C);

		TerminalWrite(Tab);
		TerminalWrite("Unknown command, make sure it is written correctly.");
		TerminalWrite("\n\n");
	}

	TerminalShell();
}

