[BITS 16]
[ORG 0x8000]

start:
	cli
	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00
	sti
	
	call clear_screen

	mov si, msg_help0
	call print_string

	mov si, msg_help0
	call print_string

	mov si, bricos_art
	call print_string

	mov si, bricos_art1
	call print_string

	mov si, bricos_art2
	call print_string

	mov si, bricos_art3
	call print_string

	mov si, bricos_art4
	call print_string

	mov si, msg_help0
	call print_string

	mov si, msg_help0
	call print_string

mainloop:
	mov si, prompt
	call print_string

	mov di, buffer
	call get_string

	mov si, buffer
	cmp byte [si], 0
	je mainloop

	mov si, buffer
	mov di, cmd_hi
	call strcmp
	jc .helloworld

	mov si, buffer
	mov di, cmd_help
	call strcmp
	jc .help

	mov si, buffer
	mov di, cmd_clear
	call strcmp
	jc .clear

	mov si, buffer
	mov di, cmd_poweroff
	call strcmp
	jc .poweroff

	mov si, buffer + 6
	mov di, cmd_touch
	call strcmp
	jc .touch

	mov si, buffer
	mov di, cmd_rm
	call strcmp
	jc .rm

	mov si, buffer
	mov di, cmd_nano
	call strcmp
	jc .nano

	mov si, buffer
	mov di, cmd_mv
	call strcmp
	jc .mv

	mov si, badcommand
	call print_string
	jmp mainloop

.helloworld:
	mov si, msg_helloworld
	call print_string


mov ah, 0x0E
	mov al, 0x08
	int 10h
	jmp mainloop

.help:
	mov si, msg_help
	call print_string

	mov si, msg_help0
	call print_string

	mov si, msg_help1
	call print_string

	mov si, msg_help0
	call print_string

	mov si, msg_help2
	call print_string

	mov si, msg_help0
	call print_string

	mov si, msg_help3
	call print_string

	mov si, msg_help0
	call print_string

	mov si, msg_help4
	call print_string

	mov si, msg_help0
	call print_string

	mov si, msg_help5
	call print_string

	mov si, msg_help0
	call print_string

	mov si, msg_help6
	call print_string

	mov si, msg_help0
	call print_string

	jmp mainloop

.clear:
	call clear_screen
	jmp mainloop

.poweroff:
	call poweroff

.touch:
	mov si, buffer + 6
	call create_file
	jmp mainloop

.rm:
	mov si, buffer + 3
	call find_file
	cmp di, 0
	je .notfound
	mov cx, 32
	mov al, 0
	rep stosb
	jmp mainloop

.nano:
	mov si, buffer + 5
	call find_file
	cmp di, 0
	je .notfound
	mov si, msg_edit
	call print_string
	add di, 8
	call get_string_limit
	jmp mainloop

.mv:
	mov si, buffer + 3
	call find_file
	cmp di, 0
	je .notfound
	push di
	mov si, buffer + 3

.skip:
	lodsb
	cmp al, ' '
	jne .skip
	pop di
	mov cx, 8
	rep movsb
	jmp mainloop

.notfound:
	mov si, file_not_found
	call print_string
	jmp mainloop

msg_helloworld db 'Ou sont mes maximator', 0x0D, 0x0A, 0
badcommand db 'UN BRICOLEUR DE TON ACABIT NE SAIT MEME PAS METTRE UNE COMMANDE', 0x0D, 0x0A, 0
prompt db 'barry@bricOS:~$ ', 0
cmd_hi db 'bonjour', 0
cmd_help db 'aled', 0

msg_help db 'LES COMMANDES NE SONT PAS DURES GROS BRICOLEUR:', 0x0D, 0x0A, 0
msg_help0 db ' ', 0x0D, 0x0A, 0
msg_help1 db 'bonjour ==> tu dois etre poli, dis bonjour', 0x0D, 0x0A, 0
msg_help2 db 'aled ==> si tu es perdu, je peux te guider', 0x0D, 0x0A, 0
msg_help3 db 'balayer ==> pour nettoyer ton travail de bricoleur faineant', 0x0D, 0x0A, 0
msg_help4 db 'dehorrr ==> pour eteindre le compouteur et retourner a ton etat naturel de bricoleur de cabane des bois', 0x0D, 0x0A, 0
msg_help5 db 'bricolage ==> si tu veux creer un fichier aussi vide que tes rapports textuels ou sexuels, cette commande est faite pour toi', 0x0D, 0x0A, 0
msg_help6 db 'parchemin ==> si tu gamberges assez, tu peux noter tes inspirations, tant que tu aboutis a un brigolage qualitatif', 0x0D, 0x0A, 0

bricos_art db '              BBBBB   RRRRR    III   CCCCC   ,,   OOO    SSSSS', 0x0D, 0x0A, 0
bricos_art1 db '              B    B  R   R     I   C        ,,  O   O  S', 0x0D, 0x0A, 0
bricos_art2 db '              BBBBB   RRRRR     I   C        ,,  O   O  SSSSS', 0x0D, 0x0A, 0
bricos_art3 db '              B    B  R  R      I   C        ,,  O   O      S', 0x0D, 0x0A, 0
bricos_art4 db '              BBBBB   R   R    III   CCCCC   ,,  OOO   SSSSS', 0x0D, 0x0A, 0

cmd_clear db 'balayer', 0
cmd_poweroff db 'dehorrr', 0
cmd_touch db 'bricolage', 0
cmd_rm db 'rm', 0
cmd_nano db 'parchemin', 0
cmd_mv db 'mv', 0

file_not_found db 'File Not Found', 0x0D, 0x0A, 0
msg_edit db 'Enter content: ', 0
file_table_full db 'No space left in file table', 0x0D, 0x0A, 0

file_table times 10*32 db 0

buffer times 64 db 0

clear_screen:
	mov ax, 0x0600
	mov bh, 0x07
	mov cx, 0x0000
	mov dx, 0x184F
	int 0x10

	mov ah, 0x02
	mov bh, 0
	mov dh, bh
	mov dl, bh
	int 0x10
	ret

poweroff:
	mov ax, 0x5301
	xor bx, bx
	int 0x15
	jc .fail

	mov ax, 0x5301
	mov bx, 0x0001



	mov cx, 0x0102
	int 0x15
	jc .fail

	mov ax, 0x5307
	mov bx, 0x0001
	mov cx, 0x0003
	int 0x15
	jc .fail

.fail:
	hlt
	jmp .fail

create_file:
	push ax
	push bx
	push cx
	push dx
	push si
	push di

	mov di, file_table
	mov cx, 10

.next_entry:
	cmp byte [di], 0
	je .found_free
	add di, 32
	loop .next_entry
	jmp .full

.found_free:
	mov cx, 8

.copy_name:
	lodsb
	cmp al, 0
	je .pad
	stosb
	loop .copy_name

.pad:
	mov al, ' '
	rep stosb

	mov cx, 24
	mov al, 0
	rep stosb
	jmp .done

.full:
	mov si, file_table_full
	call print_string

.done:
	pop di
	pop si
	pop dx
	pop cx
	pop bx
	pop ax
	ret

find_file:
	push cx
	push si
	push di

	mov di, file_table
	mov cx, 10

.loop:
	push si
	push di
	mov cx, 8
	repe cmpsb
	je .found
	pop di
	pop si
	add di, 32
	loop .loop
	xor di, di
	jmp .done

.found:
	pop di
	pop si

.done:
	pop di
	pop si
	pop cx

get_string_limit:
	xor cl, cl

.loop:
	mov ah, 0
	int 16h

	cmp al, 0x08
	je .backspace
	
	cmp al, 0x0D
	je .done

	cmp cl, 24
	je .loop

	mov ah, 0x0E
	int 10h

	stosb
	inc cl
	jmp .loop

.backspace:
	cmp cl, 0
	je .loop
	dec di
	mov byte [di], 0
	dec cl
	mov ah, 0x0E
	mov al, 0x08
	int 10h
	mov al, ' '
	int 10h
	mov al, 0x08
	int 10h
	jmp .loop

.done:
	mov al, 0
	stosb
	mov ah, 0x0E
	mov al, 0x0D
	int 10h
	mov al, 0x0A
	int 10h
	ret



print_string:
	lodsb ; grab a byte from si

	or al, al
	jz .done

	mov ah, 0x0E
	int 0x10 ; video
	
	jmp print_string

.done:
	ret

get_string:
	xor cl, cl

.loop:
	mov ah, 0
	int 0x16 ; keypress

	cmp al, 0x08 ; backspace
	je .backspace

	cmp al, 0x0D ; enter
	je .done

	cmp cl, 0x3F ; 63 char input
	je .loop ; only backspace if counter reach 63 (buffer overflow at 64)

	mov ah, 0x0E
	int 0x10 ; display char on screen

	stosb ; put character in buffer
	inc cl ; increment counter
	jmp .loop

.backspace:
	cmp cl, 0
	je .loop

	dec di
	mov byte [di], 0 ; delete char (replace with 0)
	dec cl ; decrement counter

	mov ah, 0x0E
	mov al, 0x08
	int 10h

	mov al, ' '
	int 0x10

	mov ah, 0x0E
	mov al, 0x08
	int 10h

	jmp .loop

.done:
	mov al, 0
	stosb

	mov ah, 0x0E
	mov al, 0X0D
	int 0x10
	mov al, 0x0A
	int 0x10

	ret

strcmp:

.loop:
	mov al, [si]
	mov bl, [di]
	cmp al, bl
	jne .notequal

	cmp al, 0


	je .done

	inc di
	inc si
	jmp .loop

.notequal:
	clc ; clear the carry flag
	ret

.done:
	stc ; set the carry flag
	ret
