[bits 16]
[org 0x7C00]

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00

gdt_start:
    dq 0x0000000000000000

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    dw 0xFFFF
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

cli
lgdt [gdt_descriptor]
mov eax, cr0
or eax, 0x1
mov cr0, eax
jmp 0x08:protected_mode_start

[bits 32]

protected_mode_start:
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    mov esp, 0x9FC00

	jmp 0x100000

hang:
    jmp hang

times 510-($-$$) db 0 ; fill the void in my heart, assembly let me be free
dw 0xAA55
