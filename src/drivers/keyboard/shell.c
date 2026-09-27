#include "shell.h"
#include "../screen/screen.h"
#include "../../kernel/utils/utils.h"
#include "../../cpu/fat/fat32.h"
#include "../../memory/mem.h"
#include "../../cpu/ports/ports.h"
#include "../../kernel/file/file.h"

static char lineBuffer[LINE_BUFFER_SIZE];
static int lineLength = 0;

static ShellCommands commands[] ={
    {"balayer",         shell_command_clear},
    {"presence",        shell_command_list},
    {"bricolage",       shell_command_create_file},
    {"dehors",          shell_command_shutdown},
    {"decapsule",       shell_command_read_file},
    {"prendNote",       shell_echo_file},
    {"SUPPRIME",        shell_file_delete},
    {"aled",            shell_command_help}
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

void shell_command_help(const char *argument){
    print_char('\n');
    printf("LES COMMANDES NE SONT PAS DURES, GROS BRICOLEUR:");
    print_char('\n');
    print_char('\n');
    printf("COMMANDE:           UTILISATION:");
    print_char('\n');
    printf("balayer         --> Pour nettoyer ton travail de bricoleur faineant.");
    print_char('\n');
    printf("presence        --> Pour lister ton bazar sur l'ordinateur.");
    print_char('\n');
    printf("bricolage       --> Pour créer un fichier aussi utile que ta carriere.");
    print_char('\n');
    printf("dehors          --> Pour eteindre le PC et toucher de l'herbe.");
    print_char('\n');
    printf("decapsule       --> Pour admirer le bricolage contenu dans un de tes fichiers.");
    print_char('\n');
    printf("prendNote       --> Note dans un fichier. Tu rateras quand meme l'examen.");
    print_char('\n');
    printf("SUPPRIME        --> Pour supprimer un bricolage dont tu aurais honte.");
    print_char('\n');
    printf("aled            --> Si tu es perdu, je peux te guider.");
    print_char('\n');
    print_char('\n');
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

void shell_command_read_file(const char *argument){
    int fd = file_open(bootSector.rootCluster, argument);
    char out[100];
    unsigned int n = file_read(fd, out, (sizeof(out) - 1));

    if(fd == -1){
        printf("Ce fichier n'existe pas, bricoleur alcoolique.");
    }

    out[n] = '\0';
    file_close(fd);
    print_char('\n');
    printf(out);
    print_char('\n');
    print_char('\n');
}

void shell_echo_file(const char *argument){
    char fileName[12];
    char text[100];

    if(strlen(argument) == 0){
        printf("Tu as bu mes maximators ? Il manque un nom de fichier");
        print_char('\n');
        return;
    }

    int i = 0;
    int j = 0;

    while(argument[i] != '>' && argument[i] != 0 && i < 11){
        fileName[i] = argument[i];
        i++;
    }

    if(argument[i] != '>'){
        printf("TU N'ECOUTES RIEN. Le format attendu est: prendNote nomDeFichier.ext>texte a ecrire dans le fichier");
        print_char('\n');
        return;
    }

    fileName[i] = '\0';
    i++;
    
    while (argument[i] != '\0'){
        text[j] = argument[i];
        i++;
        j++;
    }
    text[j] = '\0';


    if(strlen(text) == 0){
        printf("Il manque le contenu du fichier, bricoleur des fougeres");
        print_char('\n');
        return;
    }

    int file = file_open(bootSector.rootCluster, fileName);

    if(file == -1){
        fat32_create_file(bootSector.rootCluster, fileName);
    }

    file = file_open(bootSector.rootCluster, fileName);
    file_write(file, text, strlen(text));
    file_close(file);
}

void shell_file_delete(const char *argument){
    int file = file_delete(bootSector.rootCluster, argument);
    if(file == 0){
        printf("Le fichier n'existait deja pas, bricoleur des montagnes");
        print_char('\n');
        return;
    }

    printf("Ton bricolage bancal a ete supprime avec succes");
    print_char('\n');
}