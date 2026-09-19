#include "keyboard.h"
#include "shell.h"
#include "../../cpu/pic/pic.h"
#include "../../cpu/ports/ports.h"
#include "../../drivers/screen/screen.h"

static int shiftPressed = 0;

static void keyboard_callback(Registers *regs){
    unsigned char scancode = port_byte_in(KEYBOARD_DATA_PORT);

    if(scancode == SCANCODE_LEFT_SHIFT_PRESS || scancode == SCANCODE_RIGHT_SHIFT_PRESS){
        shiftPressed = 1;
        return;
    }

    else if(scancode == SCANCODE_LEFT_SHIFT_RELEASE || scancode == SCANCODE_RIGHT_SHIFT_RELEASE){
        shiftPressed = 0;
        return;
    }

    else if(scancode & 0x80){
        return;
    }

    else if(scancode == SCANCODE_BACKSPACE){
        shell_backspace();
        return;
    }
    
    else if (scancode == SCANCODE_ENTER){
        shell_enter();
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
        shell_put_char(letter);
        return;
    }
}

void init_keyboard(void){
    irq_install_handler(1, keyboard_callback);
}