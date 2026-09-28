#include "mem.h"

void copy_memory(char *source, char *dest, int no_bytes){
    int i;

    for (i = 0; i < no_bytes; i++){
        *(dest + i) = *(source + i);
    }
}