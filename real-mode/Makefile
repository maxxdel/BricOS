# Makefile for 16-bit OS project using NASM
# Bric'OS forever
# BRICOLEUW

# CONFIG

NASM = nasm
ASFLAGS = -f bin

BOOT_SRC = ./src/bootloader.asm
KERNEL_SRC = ./src/kernel.asm

BOOT_BIN = ./bin/bootloader.bin
KERNEL_BIN = ./bin/kernel.bin
IMG = ./bin/os.img

BIN_DIR=./bin


all: $(IMG)

$(BOOT_BIN): $(BOOT_SRC)
	$(NASM) $(ASFLAGS) $< -o $@

$(KERNEL_BIN): $(KERNEL_SRC)
	$(NASM) $(ASFLAGS) $< -o $@

$(IMG): $(BOOT_BIN) $(KERNEL_BIN)
	cat $(BOOT_BIN) $(KERNEL_BIN) > $(IMG)

clean:
	rm -f $(BIN_DIR)/*

run: $(IMG)
	qemu-system-i386 -fda $(IMG)
