disk_load:
    push dx

    mov ah, 0x02
    mov al, dh
    mov ch, 0x00
    mov dh, 0x00
    mov cl, 0x02

    int 0x13

    jc disk_error 

    pop dx
    ret

disk_error:
    pop dx

    mov si, disk_error_msg

.done:
    hlt

.print_error:
    lodsb
    cmp al, 0
    je .done

    mov ah, 0x0E
    int 0x10
    jmp .print_error

disk_error_msg db "DISK ERROR", 0