#include "utils.h"
#include "../../cpu/ports/ports.h"

void io_wait(){
    port_byte_out(0x80, 0);
}

void int_to_string(int number, char string[]){
    int i = 0;
    int sign = number;

    if(sign < 0){
        number = -number;
    }

    do{
        string[i++] = (number % 10) + '0';
        number /= 10;
    }while(number > 0);

    if(sign < 0){
        string[i++] = '-';
    }
    string[i] = '\0';

    reverse_string(string);
}

void reverse_string(char string[]){
    int i, j, tampon;

    for(i = 0, j = strlen(string) - 1; i < j; i++, j--){
        tampon = string[i];
        string[i] = string[j];
        string[j] = tampon;
    }
}

int strlen(char string[]){
    int length = 0;
    while(string[length] != '\0'){
        length++;
    }
    return length;
}

int compare_strings(char *a, char *b){
    int i = 0;
    
    if(strlen(a) != strlen(b)){
        return 0;
    }

    for (i = 0; a[i] != '\0'; i++){

        if(a[i] != b[i]){
            return 0; 
        }
    }

    return 1;
}