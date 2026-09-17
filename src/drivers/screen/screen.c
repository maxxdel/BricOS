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
}

// TODO: Faire une fonction print_char()

int printf(const char *str, int colonne, int ligne){
    // TODO: call doomscroll si last position
    char *videoMemory = (char *) VIDEO_MEMORY;
    int i = 0;
    int offset = (ligne * SCREEN_WIDTH + colonne) *2;

    while (str[i] != '\0'){
        videoMemory[offset + i * 2] = str[i];
        videoMemory[offset + i * 2 + 1] = WHITE_ON_BLACK;
        i++;
    }
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