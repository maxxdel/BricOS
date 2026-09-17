#include "pic.h"
#include "./ports/ports.h"

void pic_remap(){
    port_byte_out(PIC_MASTER_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    port_byte_out(PIC_SLAVE_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    port_byte_out(PIC_MASTER_DATA, 0x20);
    io_wait();
    port_byte_out(PIC_SLAVE_DATA, 0x28);
    io_wait();
    port_byte_out(PIC_MASTER_DATA, 1 << CASCADE_IRQ);
    io_wait();
    port_byte_out(PIC_SLAVE_DATA, CASCADE_IRQ);
    io_wait();
    port_byte_out(PIC_MASTER_DATA, ICW4_8086);
    io_wait();
    port_byte_out(PIC_SLAVE_DATA, ICW4_8086);

    port_byte_out(PIC_MASTER_DATA, 0);
    port_byte_out(PIC_SLAVE_DATA, 0);
}

void pic_send_end_of_interrupt(unsigned char irq){
    if (irq >= 8){
        port_byte_out(PIC_SLAVE_COMMAND, PIC_EOI);
    }
    
    port_byte_out(PIC_MASTER_COMMAND, PIC_EOI);
}