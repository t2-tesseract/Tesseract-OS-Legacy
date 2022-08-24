SRCS := $(shell find Source/ -name '*.c') 
OBJS := $(SRCS:.c=.o)

%.o: %.c
	gcc -g -ffreestanding -ISource/ -Wall -Wextra -fno-exceptions -fno-stack-protector -m32 -fno-pie -c $< -o $@

Kernel.bin: KernelEntry.o Interrupts.o $(OBJS)
	ld -o $@ -Ttext 0x1000 $^ --oformat binary -m elf_i386

OSImage.bin: Boot.bin Kernel.bin
	cat $^ > OSImage.bin

Boot.bin:
	nasm Bootloader/Main.asm -f bin -o Boot.bin

KernelEntry.o:
	nasm Source/Kernel/KernelEntry.asm -f elf -o KernelEntry.o

Interrupts.o:
	nasm Source/Include/Cpu/Idt/Interrupts.asm -f elf32 -o Interrupts.o

run: OSImage.bin
	qemu-system-i386 -fda OSImage.bin

clean:
	rm *.bin
	rm *.o
	rm $(OBJS)