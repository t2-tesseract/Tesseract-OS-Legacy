#include <Include/TextMode/TextMode.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>

void _start(){
	InitGdt();
	InitIdt();
	InitPic();

    TerminalClear(true);

	DebugWrite("GDT Loaded.\n", 0);
	DebugWrite("IDT Loaded.\n\n", 0);

	// tests
	DebugWrite("Test kernel msg\n", 0);
	DebugWrite("Test warning msg\n", 1);
	DebugWrite("Test error msg\n", 2);
	DebugWrite("Test log msg\n\n", 3);

	TerminalShell();

	EnableInterrupts;
    while(1);
}