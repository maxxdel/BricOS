#include "keyboard.h"
#include "../../cpu/pic/pic.h"
#include "../../cpu/ports/ports.h"
#include "../../drivers/screen/screen.h"

static int shiftPressed = 0;

static void keyboard_callback(Registers *regs){
    unsigned char scancode = port_byte_in(KEYBOARD_DATA_PORT);

    if(scancode == SCANCODE_LEFT_SHIFT_PRESS | scancode == SCANCODE_RIGHT_SHIFT_PRESS){
        shiftPressed = 1;
        return;
    }

    else if(scancode == SCANCODE_LEFT_SHIFT_RELEASE | scancode == SCANCODE_RIGHT_SHIFT_RELEASE){
        shiftPressed = 0;
    }

    else if(scancode & 0x80){
        return;
    }

    else if(scancode == SCANCODE_BACKSPACE){
        // Call la fonction de backspace qu'on setup plus tard dans shell
        return;
    }
    
    else if (scancode == SCANCODE_ENTER){
        print_char('\n');
        // Dire au buffer dans shell de mettre fin à la ligne avec "\0", et appeler la commande
        return;
    }

    else if(scancode >= KEYS_ON_BOARD){
        return;
    }

    else{
        char letter;

        if(scancode == 0){
            return;
        }

        else if(shiftPressed == 1){
            letter = scanCodeToAsciiAzertyShifted[(int)scancode];
        }
        else if(shiftPressed == 0){
            letter = scanCodeToAsciiAzerty[(int)scancode];
        }
        print_char(letter);
        return;
    }
}

void init_keyboard(void){
    irq_install_handler(1, keyboard_callback);
}