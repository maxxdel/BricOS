[bits 32]

global _start      ; Tell linker this is the entry point
extern kernel_main ; Tell assembler the function exists in C

section .text
_start:
    ; Optional: Setup esp to a good place (depends if already done)
    mov esp, 0x9FC00

    call kernel_main

.hang:
    jmp .hang
