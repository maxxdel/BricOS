#include "edit.h"
#include "../../drivers/keyboard/keyboard.h"
#include "../../drivers/keyboard/shell.h"
#include "../../drivers/screen/screen.h"
#include "../file/file.h"
#include "../../cpu/fat/fat32.h"
#include "../../memory/mem.h"
#include "../utils/utils.h"

InputHandler nanoHandler = {
    edit_put_char, edit_backspace, 
    edit_enter, edit_left_key, 
    edit_right_key, 
    0, 
    0, 
    edit_save, 
    edit_quit
};

static char fileBuffer[FILE_BUFFER_SIZE];
static int fileLength = 0;
static int cursorPosition = 0;
static char fileName[13];

void edit_put_char(char c){
    if (fileLength >= FILE_BUFFER_SIZE - 1){
        return;
    }

    for(int i = fileLength; i > cursorPosition; i--){
        fileBuffer[i] = fileBuffer[i - 1];
    }

    fileBuffer[cursorPosition] = c;
    fileLength++;

    for(int i = cursorPosition; i < fileLength; i++){
        print_char(fileBuffer[i]);
    }

    cursorPosition++;

    set_cursor(get_cursor() - (fileLength - cursorPosition) * 2);
}

void edit_backspace(){
    if(cursorPosition == 0){
        return;
    }

    for(int i = cursorPosition - 1; i < fileLength - 1; i++){
        fileBuffer[i] = fileBuffer[i + 1];
    }

    fileLength--;
    cursorPosition--;
    fileBuffer[fileLength] = '\0';

    set_cursor(get_cursor() - 2);

    for(int i = cursorPosition; i < fileLength; i++){
        print_char(fileBuffer[i]);
    }

    print_char(' ');
    set_cursor(get_cursor() - ((fileLength - cursorPosition) + 1) * 2);
}

void edit_enter(){
    fileBuffer[fileLength++] = '\n';
    print_char('\n');
}

void edit_save(){
    int file = file_open(bootSector.rootCluster, fileName);

    if(file == -1){
        if(!(fat32_create_file(bootSector.rootCluster, fileName))){return;}
        file = file_open(bootSector.rootCluster, fileName);
    }

    file_write(file, fileBuffer, fileLength);
    file_close(file);
}

void edit_quit(){
    InputHandler shellHandler = { shell_put_char, shell_backspace, shell_enter, 0, 0 };
    edit_save();
    keyboard_set_handler(shellHandler);
    clear_screen();
    shell_print_prompt();
}

void edit_open(const char *name){
    int len = strlen(name);
    if( len > 13){
        printf("Ce fichier ne peut pas etre bricole, le nom est trop long");
        print_char('\n');
        return;
    }

    copy_memory((char *) name, fileName, len);
    fileName[len] = '\0';

    fileLength = 0;

    int file = file_open(bootSector.rootCluster, fileName);
    if(file != -1){
        fileLength = file_read(file, fileBuffer, FILE_BUFFER_SIZE -1);
        file_close(file);
    }
    fileBuffer[fileLength] = '\0';
    clear_screen();
    printf(fileBuffer);
    keyboard_set_handler(nanoHandler);
}

void edit_left_key(){
    int cursorOffset;

    if(cursorPosition == 0){
        return;
    }
    cursorOffset = get_cursor() - 2;
    set_cursor(cursorOffset);
    cursorPosition--;
}

void edit_right_key(){
    int cursorOffset;

    if(cursorPosition == fileLength){
        return;
    }
    cursorOffset = get_cursor() + 2;
    set_cursor(cursorOffset);
    cursorPosition++;
}