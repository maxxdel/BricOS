#include "../memory/mem.h"
#include "../cpu/ports/ports.h"
#include "../drivers/screen/screen.h"


void bwicole(){
    int cursorPosition = 0;

    clear_screen();
    printf("Bric'OS 32bits incoming", 0, 0);
    cursorPosition = printf("...Petit Bricoleuw", 0, 1);
    set_cursor(cursorPosition);
}