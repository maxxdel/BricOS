#pragma once

#define VIDEO_MEMORY 0xb8000
#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 80
#define WHITE_ON_BLACK 0x0F

#define VGA_INDEX_PORT 0x3D4
#define VGA_DATA_PORT 0x3D5

void set_cursor(int offset);
int get_screen_offset(int colonne, int ligne);
void clear_screen(void);
int printf(const char *str);
int doom_scroll(int cursorOffset);
int get_cursor(void);
void print_char(char letter);
void erase_char(void);