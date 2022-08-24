#include <Include/TextMode/TextMode.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>

void _start(){
	InitGdt();
	InitIdt();
	InitPic();
    TerminalClear();
	TerminalSetColor(0x0A);
	TerminalWrite("Gdt loaded!\n");
	TerminalWrite("Idt loaded!\n");
	TerminalSetColor(0x0B);
	TerminalWrite("Welcome!\n\n");
	EnableInterrupts;
	TerminalWrite("Keyboard inputs: ");

    return;
}