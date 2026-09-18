#include "ports.h"

unsigned char port_byte_in(unsigned short port){
    unsigned char result;

    __asm__ volatile ("in %%dx, %%al" : "=a" (result) : "d" (port) : "memory");
    
    return result;
}

void port_byte_out(unsigned short port, unsigned char data){
    __asm__ volatile ("out %%al, %%dx" : : "a" (data), "d" (port) : "memory");
}