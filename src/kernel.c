#define VIDEO_MEMORY 0xb8000
#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 200
#define WHITE_ON_BLACK 0x0F

void clear_screen(){
    char *videoMemory = (char *) VIDEO_MEMORY;
    for (int i = 0;  i < SCREEN_HEIGHT * SCREEN_WIDTH * 2; i += 2){
        videoMemory[i] = ' ';
        videoMemory[i + 1] = WHITE_ON_BLACK;
    }
}

void printf(const char *str, int colonne, int ligne){
    char *videoMemory = (char *) VIDEO_MEMORY;
    int i = 0;
    int offset = (ligne * SCREEN_WIDTH + colonne) *2;

    while (str[i] != '\0'){
        videoMemory[offset + i * 2] = str[i];
        videoMemory[offset + i * 2 + 1] = WHITE_ON_BLACK;
        i++;
    }
    
}

void main(){
    clear_screen();
    printf("Bric'OS 32bits incoming", 0, 0);
    printf("...Petit Bricoleuw", 3, 9);
}