#include <Include/TextMode/TextMode.h>
#include <Include/TextMode/Printk.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>
#include "Log.h"

void Start(){
    InitGdt();
	InitIdt();
	InitPic();
    InitDynamicMem();

    TerminalClear();

	DebugWrite("GDT Loaded.\n", 0);
	DebugWrite("IDT Loaded.\n\n", 0);

	if (InitVfs() != 0) {
        DebugWrite("Error! Failed to init VFS!\n\n", 2);
    } else {
		DebugWrite("VFS Loaded.\n\n", 0);
	}

	TerminalShell();

	EnableInterrupts;
}