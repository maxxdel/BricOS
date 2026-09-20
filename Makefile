CC = i686-elf-gcc
CFLAGS = -ffreestanding -Wall -Wextra
LD = i686-elf-ld
NASM = nasm

all:
	$(NASM) -f bin ./src/boot/bootloader.asm -o ./bin/bootloader.bin
	$(NASM) ./src/boot/kernel_entry.asm -f elf -o ./bin/kernel_entry.o
	$(NASM) ./src/cpu/isr.asm -f elf -o ./bin/isr.o
	$(CC) $(CFLAGS) -c ./src/kernel/kernel.c -o ./bin/kernel.o
	$(CC) $(CFLAGS) -c ./src/kernel/utils/utils.c -o ./bin/utils.o
	$(CC) $(CFLAGS) -c ./src/cpu/idt/idt.c -o ./bin/idt.o
	$(CC) $(CFLAGS) -c ./src/cpu/pic/pic.c -o ./bin/pic.o
	$(CC) $(CFLAGS) -c ./src/cpu/ports/ports.c -o ./bin/ports.o
	$(CC) $(CFLAGS) -c ./src/cpu/pit/pit.c -o ./bin/pit.o
	$(CC) $(CFLAGS) -c ./src/cpu/ata/ata.c -o ./bin/ata.o
	$(CC) $(CFLAGS) -c ./src/drivers/screen/screen.c -o ./bin/screen.o
	$(CC) $(CFLAGS) -c ./src/drivers/keyboard/keyboard.c -o ./bin/keyboard.o
	$(CC) $(CFLAGS) -c ./src/drivers/keyboard/shell.c -o ./bin/shell.o
	$(CC) $(CFLAGS) -c ./src/memory/mem.c -o ./bin/mem.o
	$(LD) -o ./bin/kernel.bin -Ttext 0x1000 \
		./bin/kernel_entry.o \
		./bin/kernel.o \
		./bin/utils.o \
		./bin/idt.o \
		./bin/pic.o \
		./bin/pit.o \
		./bin/ata.o \
		./bin/ports.o \
		./bin/screen.o \
		./bin/keyboard.o \
		./bin/shell.o \
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