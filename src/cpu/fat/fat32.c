#include "fat32.h"
#include "../ata/ata.h"
#include "../../drivers/screen/screen.h"
#include "../../kernel/utils/utils.h"
#include "../../memory/mem.h"

static Fat32BootSector bootSector;

void fat32_init(void){
    unsigned short buffer[256];
    ata_read_sector(FAT32_PARTITION_START_LBA, 1, buffer);
    copy_memory((char *) buffer, (char *) &bootSector, sizeof(Fat32BootSector));
    char randomString[100];
    int_to_string(bootSector.bytesPerSector, randomString);
    printf(randomString);
    print_char('\n');
    int_to_string(bootSector.sectorsPerCluster, randomString);
    printf(randomString);
}