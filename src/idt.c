#include "idt.h"

__attribute__((aligned(0x10)))
IdtEntry idt[IDT_ENTRIES];

IdtRegistery idtr;

void idt_set_descriptor(int num, unsigned int isr, unsigned char flags){
    IdtEntry *descriptor = &idt[num];

    descriptor->isrLow                  = (unsigned short)isr & 0xffff;
    descriptor->segmentSelector         = KERNEL_CODE_SEGMENT;
    descriptor->zero                    = 0;
    descriptor->typesAndAttributes      = flags;
    descriptor->isrHigh                 = (unsigned short)isr >> 16;
}

void set_idt(){
    idtr.limit = (unsigned int)(IDT_ENTRIES * sizeof(IdtEntry) -1);
    idtr.base = (unsigned short)&idt;
    __asm__ volatile ("lidt %0" : : "m"(idtr));
}