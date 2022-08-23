#include <Include/TextMode/TextMode.h>
#include <Include/Cpu/Gdt/Gdt.h>

void _start(){
	InitGdt();
    TerminalClear();
	TerminalSetColor(0x0A);
	TerminalWrite("Gdt loaded!\n");
	TerminalSetColor(0x0B);
	TerminalWrite("Welcome!\n");

    return;
}