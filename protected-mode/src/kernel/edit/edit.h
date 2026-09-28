#pragma once

/*
Ok, normalement je commente le moins possible, mais là il faut. J'ai
un buffer anormalement gros, on peut pas écrire de fichier de plus de 
2048 dans cet éditeur, parce que j'ai eu la FLEMME de faire malloc. C'est
un point à faire dans le futur, mais là je voulais avancer, donc y'a un
buffer horrible, et pas de gestion dynamique. Si quelqu'un pass par là, 
c'est la plus gross honte de mon projet, mais j'ai réalisé l'utilité un 
peu tard. Bon courage pour fix. */
// PS : Ha d'ailleurs cet éditeur c'est le bordel, la moitié des trucs marchent pas ou explosent. Le footer bug. 
// Good luck.

#define FILE_BUFFER_SIZE 2048

#define GUTTER_WIDTH 3
#define TEXT_LEFT    GUTTER_WIDTH
#define TEXT_TOP     1
#define TEXT_WIDTH   (SCREEN_WIDTH - GUTTER_WIDTH - 3)
#define TEXT_HEIGHT  (SCREEN_HEIGHT - 1)

void edit_put_char(char c);
void edit_backspace(void);
void edit_enter(void);
void edit_save(void);
void edit_quit(void);
void edit_open(const char *name);
void edit_left_key(void);
void edit_right_key(void);
void edit_key_up(void);
void edit_key_down(void);
