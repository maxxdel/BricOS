#pragma once

#include "idt.h"

// Programmable Interrupt Controller: https://wiki.osdev.org/8259_PIC
// Faut que je rebosse cette partie pour bien comprendre les controlleurs PIC

#define PIC_MASTER_COMMAND 0x20
#define PIC_MASTER_DATA (PIC_MASTER_COMMAND + 1)
#define PIC_SLAVE_COMMAND 0xA0
#define PIC_SLAVE_DATA (PIC_SLAVE_COMMAND + 1)

#define ICW1_ICW4	    0x01
#define ICW1_SINGLE	    0x02
#define ICW1_INTERVAL4	0x04
#define ICW1_LEVEL	    0x08
#define ICW1_INIT	    0x10
#define ICW4_8086	    0x01
#define ICW4_AUTO	    0x02
#define ICW4_BUF_SLAVE	0x08
#define ICW4_BUF_MASTER	0x0C
#define ICW4_SFNM	    0x10

#define CASCADE_IRQ 2

#define PIC_EOI 0x20

typedef void (*IrqHandler)(Registers *regs);

void pic_remap(void);
void pic_send_end_of_interrupt(unsigned char irq);
void irq_install_handler(int irq, IrqHandler handler);
void irq_handler(Registers *regs);