SRCS := $(shell find Source/ -name '*.c') 
OBJS := $(SRCS:.c=.o)

%.o: %.c
	gcc -g -ffreestanding -ISource/ -Wall -Wextra -fno-exceptions -fno-stack-protector -m32 -fno-pie -c $< -o $@

Kernel.bin: KernelEntry.o Interrupts.o X86.o $(OBJS)
	ld -o $@ -Ttext 0x1000 $^ --oformat binary -m elf_i386

OSImage.img: Boot.bin Kernel.bin
	cat $^ > OSImage.img
	Je mcopy -i OSImage.img Kernel.bin "::Kernel.bin"

Boot.bin:
	nasm Bootloader/Main.asm -f bin -o Boot.bin

KernelEntry.o:
	nasm Source/Kernel/KernelEntry.asm -f elf -o KernelEntry.o

Interrupts.o:
	nasm Source/Include/Cpu/Idt/Interrupts.asm -f elf32 -o Interrupts.o

X86.o:
	nasm Source/Include/X86/X86.asm -f elf32 -o X86.o

run: OSImage.img
	qemu-system-i386 -debugcon stdio -fda OSImage.img

clean:
	rm *.bin
	rm *.o
	rm *.img
	rm *.iso
	rm Source/Include/Fat/Fat.o
	rm Source/Include/Fat/Disk.o
	rm $(OBJS)