#include "idt.h"
#include "../../drivers/screen/screen.h"

__attribute__((aligned(0x10)))
IdtEntry idt[IDT_ENTRIES];

IdtRegistery idtr;

static const char *exceptionMessages[32] = {
    "Division By Zero", "Debug", "Non Maskable Interrupt", "Breakpoint",
    "Into Detected Overflow", "Out of Bounds", "Invalid Opcode", "No Coprocessor",
    "Double Fault", "Coprocessor Segment Overrun", "Bad TSS", "Segment Not Present",
    "Stack Fault", "General Protection Fault", "Page Fault", "Unknown Interrupt",
    "Coprocessor Fault", "Alignment Check", "Machine Check", "SIMD Floating-Point Exception",
    "Virtualization Exception", "Control Protection Exception",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved",
    "Reserved", "Reserved", "Reserved", "Reserved", "Reserved"
};

void idt_set_descriptor(int num, unsigned int isr, unsigned char flags){
    IdtEntry *descriptor = &idt[num];

    descriptor->isrLow                  = (unsigned short)isr & 0xffff;
    descriptor->segmentSelector         = KERNEL_CODE_SEGMENT;
    descriptor->zero                    = 0;
    descriptor->typesAndAttributes      = flags;
    descriptor->isrHigh                 = (unsigned short)isr >> 16;
}

void set_idt(){
    idtr.limit = (unsigned short)(IDT_ENTRIES * sizeof(IdtEntry) -1);
    idtr.base = (unsigned int)&idt;
    __asm__ volatile ("lidt %0" : : "m"(idtr));
}

void idt_install(){
    extern void isr0(void);
    extern void isr1(void);
    extern void isr2(void);
    extern void isr3(void);
    extern void isr4(void);
    extern void isr5(void);
    extern void isr6(void);
    extern void isr7(void);
    extern void isr8(void);
    extern void isr9(void);
    extern void isr10(void);
    extern void isr11(void);
    extern void isr12(void);
    extern void isr13(void);
    extern void isr14(void);
    extern void isr15(void);
    extern void isr16(void);
    extern void isr17(void);
    extern void isr18(void);
    extern void isr19(void);
    extern void isr20(void);
    extern void isr21(void);
    extern void isr22(void);
    extern void isr23(void);
    extern void isr24(void);
    extern void isr25(void);
    extern void isr26(void);
    extern void isr27(void);
    extern void isr28(void);
    extern void isr29(void);
    extern void isr30(void);
    extern void isr31(void);
    extern void irq32(void);
    extern void irq33(void);
    extern void irq34(void);
    extern void irq35(void);
    extern void irq36(void);
    extern void irq37(void);
    extern void irq38(void);
    extern void irq39(void);
    extern void irq40(void);
    extern void irq41(void);
    extern void irq42(void);
    extern void irq43(void);
    extern void irq44(void);
    extern void irq45(void);
    extern void irq46(void);
    extern void irq47(void);

    static void *idtStubTable[IDT_ENTRIES_USED] = {
        isr0, isr1, isr2, isr3, isr4, isr5, isr6, isr7,
        isr8, isr9, isr10, isr11, isr12, isr13, isr14, isr15,
        isr16, isr17, isr18, isr19, isr20, isr21, isr22, isr23,
        isr24, isr25, isr26, isr27, isr28, isr29, isr30, isr31,
        irq32, irq33, irq34, irq35, irq36, irq37, irq38, irq39,
        irq40, irq41, irq42, irq43, irq44, irq45, irq46, irq47
    };

    for (int i = 0; i < IDT_ENTRIES_USED; i++){
        idt_set_descriptor(i, (unsigned int)idtStubTable[i], 0x8E);
    }

    set_idt();
}

void isr_handler(Registers *regs){
    printf(exceptionMessages[regs->intNo], 0, 0);

    for(;;){
        __asm__ volatile ("hlt");
    }
}