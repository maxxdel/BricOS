#pragma once

#define KERNEL_CODE_SEGMENT 0x08
#define IDT_ENTRIES 256
#define IDT_ENTRIES_USED 48

typedef struct IDT_ENTRY IdtEntry;
struct IDT_ENTRY{
    unsigned short isrLow;
    unsigned short segmentSelector;
    unsigned char zero;
    unsigned char typesAndAttributes;
    unsigned short isrHigh;
}__attribute__((packed));

typedef struct IDT_REG IdtRegistery;
struct IDT_REG{
    unsigned short limit;
    unsigned int base;
}__attribute__((packed));

typedef struct REGISTERS Registers;
struct REGISTERS{
    unsigned int ds;
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;
    unsigned int intNo, errCode;
    unsigned int eip, cs, eflags;
};

extern IdtEntry idt[IDT_ENTRIES];
extern IdtRegistery idtr;

void idt_set_descriptor(int num, unsigned int isr, unsigned char flags);
void set_idt(void);
void idt_install(void);
void isr_handler(Registers *regs);