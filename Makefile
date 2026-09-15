# J'ai eu des galères en bossant sur mac, refaire un makefile clean à la fin
# Juste là c'est plus lisible/modifiable si je dois changer d'outil
# Et comme une fois sur deux ça marche pas faut que j'isole les commandes, mais c'est pas ma prio
# TODO: Refaire un magnifique makefile

all: 
	nasm -f bin ./src/bootloader.asm -o ./bin/bootloader.bin
	nasm ./src/kernel_entry.asm -f elf -o ./bin/kernel_entry.o
	i686-elf-gcc -ffreestanding -c ./src/kernel.c -o ./bin/kernel.o
	i686-elf-ld -o ./bin/kernel.bin -Ttext 0x1000 ./bin/kernel_entry.o ./bin/kernel.o --oformat binary
#Pour remplir le kernel pendant les tests
	dd if=./bin/kernel.bin of=./bin/kernel_padded.bin bs=512 conv=sync
# Voir ligne 42 bootloader
	truncate -s 7680 ./bin/kernel_padded.bin
	cat ./bin/bootloader.bin ./bin/kernel_padded.bin > ./bin/os.img

clean:
	rm -rf ./bin/*

run:
	qemu-system-i386 ./bin/os.img