#include <Include/TextMode/TextMode.h>
#include <Include/TextMode/Printk.h>
#include <Include/Cpu/Gdt/Gdt.h>
#include <Include/Cpu/Idt/Idt.h>
#include <Include/Graphics/Graphics.h>
#include <Include/Vfs/Vfs.h>

void _start(){
    Start();

    struct Folder *usr = CreateFolder("usr");
    FolderCreateFile("usr", "Readme.txt");

    while(1);
}