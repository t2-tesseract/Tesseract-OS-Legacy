#include <Include/TextMode/TextMode.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>

void _start(){
	InitGdt();
	InitIdt();
	InitPic();
    TerminalClear();
	TerminalSetColor(0x02);
	TerminalWrite("Kernel: ");
	TerminalSetColor(0x0F);
	TerminalWrite("GDT Loaded!\n");
	TerminalSetColor(0x02);
	TerminalWrite("Kernel: ");
	TerminalSetColor(0x0F);
	TerminalWrite("IDT Loaded!\n\n");
	EnableInterrupts;
	TerminalSetColor(0x0B);
	TerminalWrite("Tesseract$ ");
	TerminalSetColor(0x0F);

    while(1);
}