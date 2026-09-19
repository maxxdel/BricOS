#include "shell.h"
#include "../screen/screen.h"

static char lineBuffer[LINE_BUFFER_SIZE];
static int lineLength = 0;

void shell_put_char(char c){
    
    if (lineLength >= LINE_BUFFER_SIZE - 1){
        return;
    }

    lineBuffer[lineLength++] = c;
    print_char(c);
}

void shell_backspace(){

    if(lineLength == 0){
        return;
    }

    lineBuffer[--lineLength] = '\0';
    erase_char();
}

void shell_enter(){
    lineBuffer[lineLength] = '\0';
    print_char('\n');
    lineLength = 0;
}