[BITS 32]
[extern bricole]
[extern __bss_start]
[extern _end]

cld
mov edi, __bss_start
mov ecx, _end
sub ecx, edi
xor eax, eax
rep stosb

call bricole
jmp $