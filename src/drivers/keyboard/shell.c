#include "shell.h"
#include "../screen/screen.h"
#include "../../kernel/utils/utils.h"

static char lineBuffer[LINE_BUFFER_SIZE];
static int lineLength = 0;

static void shell_print_prompt(void){
    printf("bricOS");
    printf(" - ");
}

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

    if((compare_strings(lineBuffer, "balayer")) == 1){
        clear_screen();
    }

    else if((lineLength == 0)){}

    else{
        printf("UN BRICOLEUR DE TON ACABIT NE SAIT MEME PAS METTRE UNE COMMANDE");
        print_char('\n');
    }

    lineLength = 0;
    shell_print_prompt();
}