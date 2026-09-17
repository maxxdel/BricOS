CC = i686-elf-gcc
CFLAGS = -ffreestanding -Wall -Wextra
LD = i686-elf-ld
NASM = nasm

all:
	$(NASM) -f bin ./src/boot/bootloader.asm -o ./bin/bootloader.bin
	$(NASM) ./src/boot/kernel_entry.asm -f elf -o ./bin/kernel_entry.o
	$(NASM) ./src/cpu/isr.asm -f elf -o ./bin/isr.o
	$(CC) $(CFLAGS) -c ./src/kernel/kernel.c -o ./bin/kernel.o
	$(CC) $(CFLAGS) -c ./src/cpu/idt.c -o ./bin/idt.o
	$(CC) $(CFLAGS) -c ./src/cpu/pic.c -o ./bin/pic.o
	$(CC) $(CFLAGS) -c ./src/cpu/ports/ports.c -o ./bin/ports.o
	$(CC) $(CFLAGS) -c ./src/drivers/screen/screen.c -o ./bin/screen.o
	$(CC) $(CFLAGS) -c ./src/memory/mem.c -o ./bin/mem.o
	$(LD) -o ./bin/kernel.bin -Ttext 0x1000 \
		./bin/kernel_entry.o \
		./bin/kernel.o \
		./bin/idt.o \
		./bin/pic.o \
		./bin/ports.o \
		./bin/screen.o \
		./bin/mem.o \
		./bin/isr.o \
		--oformat binary
#Pour remplir le kernel pendant les tests
	dd if=./bin/kernel.bin of=./bin/kernel_padded.bin bs=512 conv=sync
# Voir ligne 42 bootloader
	truncate -s 7680 ./bin/kernel_padded.bin
	cat ./bin/bootloader.bin ./bin/kernel_padded.bin > ./bin/os.img

clean:
	rm -rf ./bin/*

run:
	qemu-system-i386 ./bin/os.img