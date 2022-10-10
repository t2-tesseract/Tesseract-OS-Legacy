#include <Include/TextMode/TextMode.h>
#include <Include/TextMode/Printk.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>
#include <Include/Vfs/Vfs.h>

void _start(){
    Start();

    if (InitVfs() != 0) {
        return -1;
        asm("hlt");
    } 

    while(1);
}