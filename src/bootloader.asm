[BITS 16]
[org 0x7C00]

; TODO: ajouter KERNEL_OFFSET et BOOT_DRIVE

start:
	mov bp, 0x9000
	mov sp, bp
	mov si, message

print_loop:
    lodsb
    cmp al, 0
    je done

    mov ah, 0x0E
	sti
    int 0x10

    jmp print_loop

done:
	cli
	lgdt [gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax

	jmp CODE_SEG:protected_mode_start
 
 message db "Chargement du bootloader en mode reel reussi", 0

%include "./src/gdt.asm"
%include "./src/print_pm.asm"

; TODO: Inclure le kernel_entry.asm et le call

[BITS 32]

protected_mode_start:

	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	mov ebp, 0x9000
	mov esp, ebp

	mov ebx, MSG_PROTECTED_MODE
	call print_string_pm

	MSG_PROTECTED_MODE db "Succesfully entered the protected mode", 0

times 510-($-$$) db 0
dw 0xAA55