#include <Include/TextMode/TextMode.h>
#include <Include/TextMode/Printk.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>
#include <Include/Fat/Fat.h>

void _start(){
	DISK disk;


	InitGdt();
	InitIdt();
	InitPic();

    TerminalClear();

	DebugWrite("GDT Loaded.\n", 0, true);
	DebugWrite("IDT Loaded.\n\n", 0, true);

	TerminalShell();

	EnableInterrupts;

	FatFile* fd = FatOpen(&disk, "/");
    FatDirectoryEntry entry;
    int i = 0;
    while(FatReadEntry(&disk, fd, &entry) && i++ < 5){
        Printk("    ");
        for(int i = 0; i < 11; i++)
            PutChar(entry.Name[i]);
        Printk("\r\n");
    }
    FatClose(fd);

    while(1);
}