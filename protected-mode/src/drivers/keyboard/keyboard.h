#pragma once

// https://wiki.osdev.org/PS/2_Keyboard

#define KEYBOARD_DATA_PORT           0x60

#define SCANCODE_LEFT_SHIFT_PRESS    0x2A
#define SCANCODE_LEFT_SHIFT_RELEASE  0xAA
#define SCANCODE_RIGHT_SHIFT_PRESS   0x36
#define SCANCODE_RIGHT_SHIFT_RELEASE 0xB6

#define SCANCODE_LEFT_CTRL_RELEASE   0x9D
#define SCANCODE_LEFT_CTRL_PRESS     0x1D

#define SCANCODE_UPPER_KEY           0x48
#define SCANCODE_LOWER_KEY           0x50
#define SCANCODE_LEFT_KEY            0x4B
#define SCANCODE_RIGHT_KEY           0x4D

#define SCANCODE_BACKSPACE           0x0E
#define SCANCODE_ENTER               0x1C

#define SCANCODE_A                   0x10
#define SCANCODE_S                   0x1F
#define SCANCODE_X                   0x2D

#define KEYS_ON_BOARD                58


// '|' pour les accents, cédilles, etc et caractère ne pouvant pas être représentés dans le type spécifié
static const char scanCodeToAsciiAzerty[KEYS_ON_BOARD] = {
    0, '@', '&', '|', '\"', '\'', '(', '|', '|', '!', '|', '|', ')', '-', '\b',
    '\t', 'a', 'z', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '|', '$', '\n',
    0, 'q', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', 'm', '|', '`', 
    0, '<', 'w', 'x', 'c', 'v', 'b', 'n', ',', ';', ':', '=', 0,
    0, 0, ' '
};

static const char scanCodeToAsciiAzertyShifted[KEYS_ON_BOARD] = {
    0, '#', '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '|', '_', '\b',
    '\t', 'A', 'Z', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '|', '*', '\n',
    0, 'Q', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', 'M', '%', '|', 
    0, '>', 'W', 'X', 'C', 'V', 'B', 'N', '?', '.', '/', '+', 0,
    0, 0, ' '
};

typedef struct HANDLER InputHandler;
struct HANDLER{
    void (*put_char)    (char c);
    void (*backspace)   (void);
    void (*enter)       (void);
    void (*left)        (void);
    void (*right)       (void);
    void (*up)          (void);
    void (*down)        (void);
    void (*save)        (void);
    void (*quit)        (void);
    void (*start)       (void);
};

void init_keyboard(void);
void keyboard_set_handler(InputHandler setHandler);
InputHandler keyboard_get_handler(void);