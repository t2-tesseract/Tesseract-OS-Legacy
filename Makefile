SRCS := $(shell find Source/ -name '*.c') 
OBJS := $(SRCS:.c=.o)

%.o: %.c
	gcc -g -ffreestanding -ISource/ -Wall -Wextra -fno-exceptions -fno-stack-protector -m32 -fno-pie -c $< -o $@

Kernel.bin: KernelEntry.o Interrupts.o Idt.o $(OBJS)
	ld -o $@ -Ttext 0x1000 $^ --oformat binary -m elf_i386

OSImage.img: Boot.bin Kernel.bin
	# cat $^ > OS.img
	cd Build
	cat Boot.bin Kernel.bin | dd of=OS.img bs=512 conv=notrunc
	dd if=/dev/zero of=OSImage.img bs=512 count=2880
	mkfs.fat -F 12 -n "DISK" OSImage.img
	dd if=OS.img of=OSImage.img conv=notrunc
	# mcopy -i OSImage.img Kernel.bin "::Kernel.bin"
	cd..

Boot.bin:
	nasm Bootloader/Main.asm -f bin -o Boot.bin

KernelEntry.o:
	nasm Source/Kernel/KernelEntry.asm -f elf -o KernelEntry.o

Interrupts.o:
	nasm Source/Include/Cpu/Idt/Interrupts.asm -f elf32 -o Interrupts.o

Idt.o:
	nasm Source/Include/Cpu/Idt/Idt.asm -f elf32 -o Idt.o

run: OSImage.img
	qemu-system-i386 -debugcon stdio -fda Build/OSImage.img

clean:
	# rm *.bin
	# rm *.o
	# rm *.img
	rm Idt.o Interrupts.o KernelEntry.o Kernel.bin Build/OSImage.img OS.img
	rm $(OBJS)
