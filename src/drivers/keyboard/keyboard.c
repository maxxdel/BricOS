#include "keyboard.h"
#include "shell.h"
#include "../../cpu/pic/pic.h"
#include "../../cpu/ports/ports.h"
#include "../../drivers/screen/screen.h"

static int shiftPressed = 0;
static int ctrlPressed  = 0;

static InputHandler keyboardHandler = {shell_put_char, shell_backspace, shell_enter, 0, 0};

void keyboard_set_handler(InputHandler setHandler){
    keyboardHandler = setHandler;
}

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

    else if(scancode == SCANCODE_LEFT_CTRL_PRESS){
        ctrlPressed = 1;
        return;
    }

    else if(scancode == SCANCODE_LEFT_CTRL_RELEASE){
        ctrlPressed = 0;
        return;
    }

    else if(scancode & 0x80){
        return;
    }

    else if(scancode == SCANCODE_BACKSPACE){
        keyboardHandler.backspace();
        return;
    }
    
    else if (scancode == SCANCODE_ENTER){
        keyboardHandler.enter();
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

        // Ça fait tout buger, je la retravaille quand j'ai le temps
        /*else if(ctrlPressed && scancode == SCANCODE_A){
            int cursor = get_cursor();
            set_cursor(cursor - (cursor % (SCREEN_WIDTH * 2)) + 18);
            return; //probablement le return qui casse v'la les trucs d'ailleurs
        }*/

        else if (ctrlPressed && scancode == SCANCODE_S){
            if(keyboardHandler.save){
                keyboardHandler.save();
            }  
            return;
        }

        else if (ctrlPressed && scancode == SCANCODE_X){
            if(keyboardHandler.quit){
                keyboardHandler.quit();
            }
            return;
        }

        else if(shiftPressed == 1){
            letter = scanCodeToAsciiAzertyShifted[(int)scancode];
        }
        else if(shiftPressed == 0){
            letter = scanCodeToAsciiAzerty[(int)scancode];
        }
        keyboardHandler.put_char(letter);
        return;
    }
}

void init_keyboard(void){
    irq_install_handler(1, keyboard_callback);
}

InputHandler keyboard_get_handler(){
    return keyboardHandler;
}