[BITS 16]
[ORG 0x7C00]

start:
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7C00
	sti

	mov ah, 0x02
	mov al, 15
	mov ch, 0
	mov cl, 2
	mov dh, 0
	mov dl, 0
	mov bx, 0x8000
	int 0x13

	jc disk_error

	jmp 0x0000:0x8000   

disk_error:
	mov si, error_msg
	call print_string
	hlt
	jmp $

print_string:
	lodsb
	or al, al
	jz .done
	mov ah, 0x0E
	int 10h
	jmp print_string
.done:
	ret

error_msg db "Disk read error", 0
times 510 - ($ - $$) db 0
dw 0xAA55
