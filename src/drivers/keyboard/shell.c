#include "shell.h"
#include "../screen/screen.h"
#include "../../kernel/utils/utils.h"
#include "../../cpu/fat/fat32.h"
#include "../../memory/mem.h"
#include "../../cpu/ports/ports.h"

static char lineBuffer[LINE_BUFFER_SIZE];
static int lineLength = 0;

static ShellCommands commands[] ={
    {"balayer",     shell_command_clear},
    {"presence",    shell_command_list},
    {"bricolage",   shell_command_create_file},
    {"dehors",      shell_command_shutdown}
};
    

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

void shell_parse_line(char *command, char *argument){
    int spaceIndex = -1;

    for(int i = 0; i < lineLength; i++){
        if(lineBuffer[i] == ' '){
            spaceIndex = i;
            break;
        }
    }

    if(spaceIndex == -1){
        copy_memory(lineBuffer, command, lineLength);
        command[lineLength] = '\0';
        argument[0] = '\0';
    }

    else{
        copy_memory(lineBuffer, command, spaceIndex);
        command[spaceIndex] = '\0';

        int argLength = lineLength - spaceIndex - 1;
        copy_memory(lineBuffer + spaceIndex + 1, argument, argLength);
        argument[argLength] = '\0';
    }
}

void shell_enter(){
    lineBuffer[lineLength] = '\0';
    print_char('\n');
    char inputCommand[MAX_COMMAND_SIZE];
    char argument[LINE_BUFFER_SIZE - MAX_COMMAND_SIZE];

    shell_parse_line(inputCommand, argument);

    int found = 0;
    for(int i = 0; i < (sizeof(commands) / sizeof(commands[0])); i++){
        if(compare_strings(inputCommand, commands[i].command) == 1){
            commands[i].handler(argument);
            found = 1;
            break;
        }
    }

    if(!found && lineLength > 0){
        printf("UN BRICOLEUR DE TON ACABIT NE SAIT MEME PAS METTRE UNE COMMANDE");
        print_char('\n');
    }
    lineLength = 0;
    shell_print_prompt();
}

void shell_command_clear(const char *argument){
    clear_screen();
}

void shell_command_list(const char *argument){
    print_char('\n');
    printf("Voici les fichiers, gros bricoleur : ");
    print_char('\n');
    print_char('\n');
    fat32_get_root_dir();
    print_char('\n');
}

void shell_command_create_file(const char *argument){

    if(strlen(argument) == 0){
        printf("Tu as bu mes maximators ? Il manque un nom de fichier");
        print_char('\n');
        return;
    }

    if(strlen(argument) > 12){
        printf("Ton nom de fichier est trop long, bricoleur malhabile");
        return;
    }

    int created = fat32_create_file(bootSector.rootCluster, argument);
    
    if(created == 0){
        printf("Une erreur est survenue, probablement de ta faute, petit bricoleur");
        print_char('\n');
        return;
    }
    
    printf("Le fichier ");
    printf(argument);
    printf(" a ete bricole en grande pompe.");
    print_char('\n');
    return;
}

void shell_command_shutdown(const char *argument){
    printf("À bientot... Petit bricoleur");
    port_word_out(0x604, 0x2000);
    port_word_out(0xB004, 0x2000);
}