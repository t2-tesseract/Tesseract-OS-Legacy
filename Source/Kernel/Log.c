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

	DebugWrite("GDT Loaded.\n", 0, true);
	DebugWrite("IDT Loaded.\n\n", 0, true);

	TerminalShell();

	EnableInterrupts;
    
}