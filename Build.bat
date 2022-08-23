@ECHO OFF

set Directory=C:\i686-elf-tools-windows
set Nasm=C:\nasm

%Nasm%\nasm.exe Bootloader/Main.asm -f bin -o Boot.bin
%Nasm%\nasm.exe Kernel/KernelEntry.asm -f elf -o KernelEntry.o

%Directory%\bin\i686-elf-gcc.exe -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-pie -c Kernel/Kernel.c -o Kernel.o

%Nasm%\nasm.exe Bootloader/Zeroes.asm -f bin -o Zeroes.bin

%Directory%\bin\i686-elf-ld.exe -o FullKernel.bin -Ttext 0x1000 KernelEntry.o Kernel.o --oformat binary -m elf_i386

cat Boot.bin FullKernel.bin Zeroes.bin > OSImage.bin