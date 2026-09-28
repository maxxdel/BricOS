#include "edit.h"
#include "../../drivers/keyboard/keyboard.h"
#include "../../drivers/keyboard/shell.h"
#include "../../drivers/screen/screen.h"
#include "../file/file.h"
#include "../../cpu/fat/fat32.h"
#include "../../memory/mem.h"
#include "../utils/utils.h"

static void edit_get_row_col(int index, int *row, int *col);
static void edit_redraw(void);
static void edit_place_cursor(void);
static void edit_insert(char c);
static int edit_line_start(int index);
static int edit_line_end(int index);
static void edit_shortcut_start(void);

InputHandler nanoHandler = {
    edit_put_char, 
    edit_backspace, 
    edit_enter, 
    edit_left_key, 
    edit_right_key, 
    edit_key_up, 
    edit_key_down, 
    edit_save, 
    edit_quit,
    edit_shortcut_start
};

InputHandler shellHandler = { 
    shell_put_char, 
    shell_backspace, 
    shell_enter, 
    shell_left_key, 
    shell_right_key,
    shell_key_up,
    shell_key_down,
    0, 
    0,
    shell_shortcut_start
};

static char fileBuffer[FILE_BUFFER_SIZE];
static int fileLength = 0;
static int cursorPosition = 0;
static char fileName[13];
static int topLine = 0;

void edit_put_char(char c){
    edit_insert(c);
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
    edit_redraw();
}

void edit_enter(){
    edit_insert('\n');
}

void edit_save(){
    int file = file_open(bootSector.rootCluster, fileName);

    if(file == -1){
        if(!(fat32_create_file(bootSector.rootCluster, fileName))){return;}
        file = file_open(bootSector.rootCluster, fileName);
    }

    file_write(file, fileBuffer, fileLength);
    file_truncate(file);
    file_close(file);
}

void edit_quit(){
    edit_save();
    keyboard_set_handler(shellHandler);
    clear_screen();
    shell_print_prompt();
}

void edit_open(const char *name){
    fileLength = 0;
    cursorPosition = 0;
    topLine = 0;

    int len = strlen(name);
    
    if( len > 12){
        printf("Ce fichier ne peut pas etre bricole, le nom est trop long");
        print_char('\n');
        return;
    }

    copy_memory((char *) name, fileName, len);
    fileName[len] = '\0';

    fileLength = 0;
    cursorPosition = 0;

    int file = file_open(bootSector.rootCluster, fileName);
    
    if(file != -1){
        fileLength = file_read(file, fileBuffer, FILE_BUFFER_SIZE -1);
        file_close(file);
    }
    
    fileBuffer[fileLength] = '\0';
    edit_redraw();
    keyboard_set_handler(nanoHandler);
}

void edit_left_key(){
    if(cursorPosition == 0){
        return;
    }
    cursorPosition--;
    edit_place_cursor();
}

void edit_right_key(){
    if(cursorPosition == fileLength){
        return;
    }
    cursorPosition++;
    edit_place_cursor();
}

static void edit_get_row_col(int index, int *row, int *col){
    int r = 0, c = 0;
    for(int i = 0; i < index; i++){
        if(fileBuffer[i] == '\n'){ r++; c = 0; }
        else { c++; if(c == SCREEN_WIDTH){ c = 0; r++; } }
    }
    *row = r;
    *col = c;
}

static void edit_redraw(void){
    char *video = (char *) VIDEO_MEMORY;
    int cursorRow, cursorCol;

    edit_get_row_col(cursorPosition, &cursorRow, &cursorCol);

    if(cursorRow < topLine){
        topLine = cursorRow;
    }
    
    else if(cursorRow >= topLine + SCREEN_HEIGHT){
        topLine = cursorRow - SCREEN_HEIGHT + 1;
    }

    clear_screen();

    int r = 0, c = 0;
    for(int i = 0; i < fileLength; i++){
        if(r >= topLine + SCREEN_HEIGHT){
            break;
        }

        if(fileBuffer[i] == '\n'){
            r++;
            c = 0;
            continue;
        }

        if(r >= topLine){
            int offset = get_screen_offset(c, r - topLine);
            video[offset]     = fileBuffer[i];
            video[offset + 1] = WHITE_ON_BLACK;
        }

        c++;
        if(c == SCREEN_WIDTH){
            c = 0;
            r++;
        }
    }

    set_cursor(get_screen_offset(cursorCol, cursorRow - topLine));
}

static void edit_place_cursor(void){
    int row, col;
    edit_get_row_col(cursorPosition, &row, &col);

    if(row < topLine || row >= topLine + SCREEN_HEIGHT){
        edit_redraw();
        return;
    }

    set_cursor(get_screen_offset(col, row - topLine));
}

static void edit_insert(char c){
    if(fileLength >= FILE_BUFFER_SIZE - 1){
        return;
    }

    for(int i = fileLength; i > cursorPosition; i--){
        fileBuffer[i] = fileBuffer[i - 1];
    }

    fileBuffer[cursorPosition] = c;
    fileLength++;
    cursorPosition++;
    fileBuffer[fileLength] = '\0';
    edit_redraw();
}

static int edit_line_start(int index){
    while(index > 0 && fileBuffer[index - 1] != '\n'){
        index--;
    }
    return index;
}

static int edit_line_end(int index){
    while(index < fileLength && fileBuffer[index] != '\n'){
        index++;
    }
    return index;
}

void edit_key_up(){
    int currentStart = edit_line_start(cursorPosition);

    if(currentStart == 0){
        return;
    }

    int column = cursorPosition - currentStart;

    int previousEnd = currentStart - 1;
    int previousStart = edit_line_start(previousEnd);
    int previousLength = previousEnd - previousStart;

    if(column > previousLength){
        column = previousLength;
    }

    cursorPosition = previousStart + column;
    edit_place_cursor();
}

void edit_key_down(){
    int currentStart = edit_line_start(cursorPosition);
    int column = cursorPosition - currentStart;

    int currentEnd = edit_line_end(cursorPosition);

    if(currentEnd == fileLength){
        return;
    }

    int nextStart = currentEnd + 1;
    int nextEnd = edit_line_end(nextStart);
    int nextLength = nextEnd - nextStart;

    if(column > nextLength){
        column = nextLength;
    }

    cursorPosition = nextStart + column;
    edit_place_cursor();
}

static void edit_shortcut_start(){
    cursorPosition = edit_line_start(cursorPosition);
    edit_place_cursor();
}