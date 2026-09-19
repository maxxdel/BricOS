#include "pit.h"
#include "../pic/pic.h"
#include "../ports/ports.h"
#include "../../kernel/utils/utils.h"
#include "../../drivers/screen/screen.h"

// https://wiki.osdev.org/Programmable_Interval_Timer

unsigned int tick = 0;

static void timer_callback(Registers *regs){
    tick ++;
    char ticksToAscii[256];
    int_to_string(tick, ticksToAscii);
}

void init_timer(unsigned int frequency){
    irq_install_handler(IRQ_TIMER, timer_callback);

    unsigned int divisor    = 1193180 / frequency;
    unsigned char low       = (unsigned char)(divisor & 0xff);
    unsigned char high      = (unsigned char)((divisor & 0xff) >> 8);

    port_byte_out(PIT_COMMAND, PIT_MODE3_SQUARE_WAVE);
    port_byte_out(PIT_DATA_0, low);
    port_byte_out(PIT_DATA_0, high);
}