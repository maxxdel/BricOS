#include "../memory/mem.h"
#include "../cpu/ports/ports.h"
#include "../cpu/idt/idt.h"
#include "../cpu/pic/pic.h"
#include "../cpu/pit/pit.h"
#include "../drivers/screen/screen.h"
#include "../drivers/keyboard/keyboard.h"

void bricole(){
    int cursorPosition = 0;

    idt_install();
    pic_remap();
    __asm__ volatile ("sti");

    clear_screen();
    printf("Bric'OS 32bits incoming", 0, 0);
    cursorPosition = printf("...Petit Bricoleuw", 0, 1);
    set_cursor(cursorPosition);
    init_timer(50);
    init_keyboard();
}