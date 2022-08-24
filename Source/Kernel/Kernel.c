#include <Include/TextMode/TextMode.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>

void _start(){
	InitGdt();
	InitIdt();
	InitPic();

    TerminalClear(true);

	TerminalSetColor(0x02);
	TerminalWrite("Kernel: ");
	TerminalSetColor(0x0F);
	TerminalWrite("GDT Loaded.\n");

	TerminalSetColor(0x02);
	TerminalWrite("Kernel: ");
	TerminalSetColor(0x0F);
	TerminalWrite("IDT Loaded.\n\n");

	TerminalShell();

	EnableInterrupts;
    while(1);
}