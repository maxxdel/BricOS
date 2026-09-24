#include "../memory/mem.h"
#include "../cpu/ports/ports.h"
#include "../cpu/idt/idt.h"
#include "../cpu/pic/pic.h"
#include "../cpu/pit/pit.h"
#include "../cpu/ata/ata.h"
#include "../cpu/fat/fat32.h"
#include "../drivers/screen/screen.h"
#include "../drivers/keyboard/keyboard.h"

void bricole(){
    idt_install();
    pic_remap();
    __asm__ volatile ("sti");
    clear_screen();
    printf("Bric'OS 32bits incoming");
    print_char('\n');
    printf("...Petit Bricoleuw");
    print_char('\n');
    print_char('\n');
    init_timer(50);
    init_keyboard();
    fat32_init();
    fat32_create_file(bootSector.rootCluster, "TEST.TXT");
    fat32_get_root_dir();
}