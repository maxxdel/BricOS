#include "screen.h"
#include "../../memory/mem.h"
#include "../../cpu/ports/ports.h"

void set_cursor(int offset){
    offset /= 2;
    port_byte_out(VGA_INDEX_PORT, 14);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset >> 8));
    port_byte_out(VGA_INDEX_PORT, 15);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset & 0xff));
}

int get_screen_offset(int colonne, int ligne){
    return (ligne * SCREEN_WIDTH + colonne) * 2;
}

void clear_screen(){
    char *videoMemory = (char *) VIDEO_MEMORY;
    for (int i = 0;  i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i += 2){
        videoMemory[i] = ' ';
        videoMemory[i + 1] = WHITE_ON_BLACK;
    }
    set_cursor(0);
}

int printf(const char *str){
    char *videoMemory = (char *) VIDEO_MEMORY;
    int i = 0;
    int offset;

    offset = get_cursor();
    offset = doom_scroll(offset);

    while (str[i] != '\0'){
        videoMemory[offset + i * 2] = str[i];
        videoMemory[offset + i * 2 + 1] = WHITE_ON_BLACK;
        i++;
    }
    set_cursor(offset + i *2);
    return(offset + i * 2);
}

int doom_scroll(int cursorOffset){
    int i;
    if (cursorOffset < SCREEN_HEIGHT * SCREEN_WIDTH * 2){
        return cursorOffset;
    }

    for(i = 1; i < SCREEN_HEIGHT; i++ ){
        copy_memory(
            (char *)(get_screen_offset(0, i) + VIDEO_MEMORY), 
            (char *)(get_screen_offset(0, (i - 1)) + VIDEO_MEMORY), 
            SCREEN_WIDTH * 2
        );
    }

    char *lastLine = (char *)(get_screen_offset(0, SCREEN_HEIGHT - 1) + VIDEO_MEMORY);
    for (i = 0; i < SCREEN_WIDTH * 2; i++){
        lastLine[i] = 0;
    }

    cursorOffset -= 2 * SCREEN_WIDTH;

    return cursorOffset;
}

int get_cursor(){
    int offset; 
    unsigned char offsetHigh, offsetLow;

    port_byte_out(VGA_INDEX_PORT, 14);
    offsetHigh = port_byte_in(VGA_DATA_PORT);
    port_byte_out(VGA_INDEX_PORT, 15);
    offsetLow = port_byte_in(VGA_DATA_PORT);

    offset = (((unsigned char)offsetHigh << 8) | offsetLow) * 2;

    return offset;
}

void print_char(char letter){
    char *videoMemory = (char *) VIDEO_MEMORY;
    int cursorOffset;

    cursorOffset = get_cursor();
    cursorOffset = doom_scroll(cursorOffset);

    if (letter == '\n'){
        cursorOffset = cursorOffset - (cursorOffset % (SCREEN_WIDTH * 2)) + (SCREEN_WIDTH * 2);
        cursorOffset = doom_scroll(cursorOffset);
    }
    
    else{
    videoMemory[cursorOffset] = letter;
    videoMemory[cursorOffset +1] = WHITE_ON_BLACK;

    cursorOffset += 2;
    cursorOffset = doom_scroll(cursorOffset);
    }

    set_cursor(cursorOffset);
}

void erase_char(){
    char *videoMemory = (char *) VIDEO_MEMORY;
    int cursorOffset;

    cursorOffset = get_cursor() - 2;
    videoMemory[cursorOffset] = ' ';
    videoMemory[cursorOffset + 1] = WHITE_ON_BLACK;

    set_cursor(cursorOffset);
}