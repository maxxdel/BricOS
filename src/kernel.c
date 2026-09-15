#define VIDEO_MEMORY 0xb8000
#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80
#define WHITE_ON_BLACK 0x0F

#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

unsigned char port_byte_in(unsigned short port){
    unsigned char result;

    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));
    
    return result;
}

void port_byte_out(unsigned short port, unsigned char data){
    __asm__("out %%al, %%dx" : : "a" (data), "d" (port));
}

void set_cursor(int offset){
    offset /= 2;
    port_byte_out(VGA_INDEX_PORT, 14);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset >> 8));
    port_byte_out(VGA_INDEX_PORT, 15);
    port_byte_out(VGA_DATA_PORT, (unsigned char)(offset & 0xff));
}

void copy_memory(char *source, char *dest, int no_bytes){
    int i;

    for (i = 0; i < no_bytes; i++){
        *(dest + i) = *(source + i);
    }
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

int printf(const char *str, int colonne, int ligne){
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

void bwicole(){
    int cursorPosition = 0;

    clear_screen();
    printf("Bric'OS 32bits incoming", 0, 0);
    cursorPosition = printf("...Petit Bricoleuw", 0, 1);
    set_cursor(cursorPosition);
}