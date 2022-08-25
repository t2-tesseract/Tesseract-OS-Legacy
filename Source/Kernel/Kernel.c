#include <Include/TextMode/TextMode.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>

void _start(){
	InitGdt();
	InitIdt();
	InitPic();

    TerminalClear();

	DebugWrite("GDT Loaded.\n", 0, true);
	DebugWrite("IDT Loaded.\n\n", 0, true);

	DebugWrite("Test kernel msg\n", 0, false);
	DebugWrite("Test warning msg\n", 1, false);
	DebugWrite("Test error msg\n", 2, false);
	DebugWrite("Test log msg\n\n", 3, false);

	TerminalShell();

	EnableInterrupts;

	// while (1)
	// {
	// 	TerminalWrite("Hello\n");
	// 	Delay(500);
	// 	TerminalWrite("Binouka\n");
	// }
	

    while(1);
}