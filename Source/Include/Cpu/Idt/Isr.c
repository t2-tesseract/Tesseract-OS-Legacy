#include <Include/Common/Common.h>
#include <Include/TextMode/TextMode.h>
#include "Isr.h"

char *ExceptionMessages[32] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "BOUND Range Exceeded",
    "Invalid Opcode",
    "Device Not Available (No Math Coprocessor)",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection",
    "Page Fault",
    "Unknown Interrupt (intel reserved)",
    "x87 FPU Floating-Point Error (Math Fault)",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void FaultHandler(Registers *r){
    if (r->int_no < 32){
        TerminalClear();
        PrintkSetColor(0x0C);

        Printk("Unhandled exception %d %s\n", r->int_no, ExceptionMessages[r->int_no]);
        Printk("  eax=%x  ebx=%x  ecx=%x  edx=%x  esi=%x  edi=%x\n",
               r->eax, r->ebx, r->ecx, r->edx, r->esi, r->edi);

        Printk("  esp=%x  ebp=%x  eip=%x  eflags=%x  cs=%x  ds=%x  ss=%x\n",
               r->esp, r->ebp, r->eip, r->eflags, r->cs, r->ds, r->ss);
        Printk("  interrupt=%x  errorcode=%x\n", r->int_no, r->err_code);
        Printk("KERNEL PANIC!\n");
        asm("hlt");
    }
}

void InitIsr(){
    InitIdtDescriptor(0, (unsigned)Exception0, 0x08, 0x8E);
    InitIdtDescriptor(1, (unsigned)Exception1, 0x08, 0x8E);
    InitIdtDescriptor(2, (unsigned)Exception2, 0x08, 0x8E);
    InitIdtDescriptor(3, (unsigned)Exception3, 0x08, 0x8E);
    InitIdtDescriptor(4, (unsigned)Exception4, 0x08, 0x8E);
    InitIdtDescriptor(5, (unsigned)Exception5, 0x08, 0x8E);
    InitIdtDescriptor(6, (unsigned)Exception6, 0x08, 0x8E);
    InitIdtDescriptor(7, (unsigned)Exception7, 0x08, 0x8E);
    InitIdtDescriptor(8, (unsigned)Exception8, 0x08, 0x8E);
    InitIdtDescriptor(9, (unsigned)Exception9, 0x08, 0x8E);
    InitIdtDescriptor(10, (unsigned)Exception10, 0x08, 0x8E);
    InitIdtDescriptor(11, (unsigned)Exception11, 0x08, 0x8E);
    InitIdtDescriptor(12, (unsigned)Exception12, 0x08, 0x8E);
    InitIdtDescriptor(13, (unsigned)Exception13, 0x08, 0x8E);
    InitIdtDescriptor(14, (unsigned)Exception14, 0x08, 0x8E);
    InitIdtDescriptor(15, (unsigned)Exception15, 0x08, 0x8E);
    InitIdtDescriptor(16, (unsigned)Exception16, 0x08, 0x8E);
    InitIdtDescriptor(17, (unsigned)Exception17, 0x08, 0x8E);
    InitIdtDescriptor(18, (unsigned)Exception18, 0x08, 0x8E);
    InitIdtDescriptor(19, (unsigned)Exception19, 0x08, 0x8E);
    InitIdtDescriptor(20, (unsigned)Exception20, 0x08, 0x8E);
    InitIdtDescriptor(21, (unsigned)Exception21, 0x08, 0x8E);
    InitIdtDescriptor(22, (unsigned)Exception22, 0x08, 0x8E);
    InitIdtDescriptor(23, (unsigned)Exception23, 0x08, 0x8E);
    InitIdtDescriptor(24, (unsigned)Exception24, 0x08, 0x8E);
    InitIdtDescriptor(25, (unsigned)Exception25, 0x08, 0x8E);
    InitIdtDescriptor(26, (unsigned)Exception26, 0x08, 0x8E);
    InitIdtDescriptor(27, (unsigned)Exception27, 0x08, 0x8E);
    InitIdtDescriptor(28, (unsigned)Exception28, 0x08, 0x8E);
    InitIdtDescriptor(29, (unsigned)Exception29, 0x08, 0x8E);
    InitIdtDescriptor(30, (unsigned)Exception30, 0x08, 0x8E);
    InitIdtDescriptor(31, (unsigned)Exception31, 0x08, 0x8E);
}

void *IrqRoutines[16] =
{
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

void IrqInstallHandler(int irq, void (*handler)(Registers *r)){
    IrqRoutines[irq] = handler;
}

void IrqUninstallHandler(int irq){
    IrqRoutines[irq] = 0;
}

void RemapIrq(void){
    Outb(0x20, 0x11);
    Outb(0xA0, 0x11);
    Outb(0x21, 0x20);
    Outb(0xA1, 0x28);
    Outb(0x21, 0x04);
    Outb(0xA1, 0x02);
    Outb(0x21, 0x01);
    Outb(0xA1, 0x01);
    Outb(0x21, 0x0);
    Outb(0xA1, 0x0);
}

void InitIrq(){
    RemapIrq();

    InitIdtDescriptor(32, (uint32_t)Irq0, 0x08, 0x8E);
    InitIdtDescriptor(33, (uint32_t)Irq1, 0x08, 0x8E);
    InitIdtDescriptor(34, (uint32_t)Irq2, 0x08, 0x8E);
    InitIdtDescriptor(35, (uint32_t)Irq3, 0x08, 0x8E);
    InitIdtDescriptor(36, (uint32_t)Irq4, 0x08, 0x8E);
    InitIdtDescriptor(37, (uint32_t)Irq5, 0x08, 0x8E);
    InitIdtDescriptor(38, (uint32_t)Irq6, 0x08, 0x8E);
    InitIdtDescriptor(39, (uint32_t)Irq7, 0x08, 0x8E);
    InitIdtDescriptor(40, (uint32_t)Irq8, 0x08, 0x8E);
    InitIdtDescriptor(41, (uint32_t)Irq9, 0x08, 0x8E);
    InitIdtDescriptor(42, (uint32_t)Irq10, 0x08, 0x8E);
    InitIdtDescriptor(43, (uint32_t)Irq11, 0x08, 0x8E);
    InitIdtDescriptor(44, (uint32_t)Irq12, 0x08, 0x8E);
    InitIdtDescriptor(45, (uint32_t)Irq13, 0x08, 0x8E);
    InitIdtDescriptor(46, (uint32_t)Irq14, 0x08, 0x8E);
    InitIdtDescriptor(47, (uint32_t)Irq15, 0x08, 0x8E);
}

void IrqHandler(Registers *r){
    void (*Handler)(Registers *r);
    Handler = IrqRoutines[r->int_no - 32];
    if (Handler)
    {
        Handler(r);
    }
    if (r->int_no >= 40)
    {
        Outb(0xA0, 0x20);
    }

    Outb(0x20, 0x20);
}

void TimerPhase(int hz){
    int Divisor = 1193180 / hz;
    Outb(0x43, 0x36);
    Outb(0x40, Divisor & 0xFF);
    Outb(0x40, Divisor >> 8);
}

int TimerTicks = 0;

void TimeHandler(Registers* r){
    TimerTicks++;
    if (TimerTicks % 18 == 0)
    {
        // DrawString("Tick!", 15, 15, 0xFFFFFF);
    }
}

void TimerInstall(){
    IrqInstallHandler(0, TimeHandler);
}