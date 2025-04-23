ENTRY(_start)

SECTIONS {
    /* Set the kernel load address to 1MB (conventional for kernels) */
    . = 0x100000;

    /* First put the multiboot header, as it needs to be within the first 8KB */
    .multiboot : {
        /* Ensure that the multiboot header is at the beginning of the file */
        *(.multiboot)
    }

    /* Text section (code) */
    .text ALIGN(4K) : {
        *(.text)
    }

    /* Read-only data */
    .rodata ALIGN(4K) : {
        *(.rodata)
        *(.rodata.*)
    }

    /* Read-write data (initialized) */
    .data ALIGN(4K) : {
        *(.data)
    }

    /* Read-write data (uninitialized) and stack */
    .bss ALIGN(4K) : {
        *(COMMON)
        *(.bss)
    }

    /* Remove unnecessary sections to make the kernel as small as possible */
    /DISCARD/ : {
        *(.comment)
        *(.eh_frame)
        *(.note.gnu.build-id)
    }
}
