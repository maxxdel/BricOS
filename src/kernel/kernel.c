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
    unsigned short writeBuffer[256];
    for(int i = 0; i < 256; i++){
        writeBuffer[i] = 0x1234;
    }
    // ata_write_sector(27, 1, writeBuffer);
    // unsigned short buffer[256];
    // ata_read_sector(27, 1, buffer);
    char randomChar[100];
    // int_to_string(buffer[256], randomChar);
    // printf(randomChar);
    fat32_init();
    unsigned int test = fat32_get_next_cluster(1);
    int_to_string(test, randomChar);
    printf(randomChar);
}