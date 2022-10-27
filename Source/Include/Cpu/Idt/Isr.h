#ifndef ISR_H
#define ISR_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    unsigned int gs, fs, es, ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int int_no, err_code;
    unsigned int eip, cs, eflags, useresp, ss;
} Registers;

void FaultHandler(Registers *r);
void IrqHandler(Registers *r);
void InitIsr();
void InitIrq();

void IrqInstallHandler(int irq, void (*handler)(Registers *r));
void IrqUninstallHandler(int irq);

void TimerInstall();

extern void Exception0();
extern void Exception1();
extern void Exception2();
extern void Exception3();
extern void Exception4();
extern void Exception5();
extern void Exception6();
extern void Exception7();
extern void Exception8();
extern void Exception9();
extern void Exception10();
extern void Exception11();
extern void Exception12();
extern void Exception13();
extern void Exception14();
extern void Exception15();
extern void Exception16();
extern void Exception17();
extern void Exception18();
extern void Exception19();
extern void Exception20();
extern void Exception21();
extern void Exception22();
extern void Exception23();
extern void Exception24();
extern void Exception25();
extern void Exception26();
extern void Exception27();
extern void Exception28();
extern void Exception29();
extern void Exception30();
extern void Exception31();

extern void Irq0();
extern void Irq1();
extern void Irq2();
extern void Irq3();
extern void Irq4();
extern void Irq5();
extern void Irq6();
extern void Irq7();
extern void Irq8();
extern void Irq9();
extern void Irq10();
extern void Irq11();
extern void Irq12();
extern void Irq13();
extern void Irq14();
extern void Irq15();

#define IRQ_BASE            0x20
#define IRQ0_TIMER          0x00
#define IRQ1_KEYBOARD       0x01
#define IRQ2_CASCADE        0x02
#define IRQ3_SERIAL_PORT2   0x03
#define IRQ4_SERIAL_PORT1   0x04
#define IRQ5_RESERVED       0x05
#define IRQ6_DISKETTE_DRIVE 0x06
#define IRQ7_PARALLEL_PORT  0x07
#define IRQ8_CMOS_CLOCK     0x08
#define IRQ9_CGA            0x09
#define IRQ10_RESERVED      0x0A
#define IRQ11_RESERVED      0x0B
#define IRQ12_AUXILIARY     0x0C
#define IRQ13_FPU           0x0D
#define IRQ14_HARD_DISK     0x0E
#define IRQ15_RESERVED      0x0F

#endif