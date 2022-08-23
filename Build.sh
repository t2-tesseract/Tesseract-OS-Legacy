nasm Bootloader/Main.asm -f bin -o Boot.bin
nasm Kernel/KernelEntry.asm -f elf -o KernelEntry.o
gcc -g -ffreestanding -Wall -Wextra -fno-exceptions -m32 -fno-pie -c Kernel/Kernel.c -o Kernel.o
nasm Bootloader/Zeroes.asm -f bin -o Zeroes.bin

i686-elf-ld -o FullKernel.bin -Ttext 0x1000 KernelEntry.o Kernel.o --oformat binary -m elf_i386

cat Boot.bin FullKernel.bin Zeroes.bin > OSImage.bin

qemu-system-i386 -m 128M -fda OSImage.bin