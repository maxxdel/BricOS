#include "../memory/mem.h"
#include "../cpu/ports/ports.h"
#include "../cpu/idt/idt.h"
#include "../cpu/pic/pic.h"
#include "../cpu/pit/pit.h"
#include "../cpu/ata/ata.h"
#include "../cpu/fat/fat32.h"
#include "../drivers/screen/screen.h"
#include "../drivers/keyboard/keyboard.h"
#include "./file/file.h"

void bricole(){
    idt_install();
    pic_remap();
    __asm__ volatile ("sti");
    clear_screen();
    init_timer(50);
    init_keyboard();
   
    fat32_init();

    print_char('\n');
    printf("                BBBBB   RRRRR    III   CCCCC   ,,   OOO    SSSSS");
    print_char('\n');
    printf("                B    B  R   R     I   C        ,,  O   O  S");
    print_char('\n');
    printf("                BBBBB   RRRRR     I   C        ,,  O   O  SSSSS");
    print_char('\n');
    printf("                B    B  R  R      I   C        ,,  O   O      S");
    print_char('\n');
    printf("                BBBBB   R   R    III   CCCCC   ,,  OOO   SSSSS");
    print_char('\n');
    print_char('\n');
    printf("Clique sur entree pour commencer a bricoler");
    print_char('\n');
}