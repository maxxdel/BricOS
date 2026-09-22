#include "fat32.h"
#include "../ata/ata.h"
#include "../../drivers/screen/screen.h"
#include "../../kernel/utils/utils.h"
#include "../../memory/mem.h"

static Fat32BootSector bootSector;

void fat32_init(void){
    unsigned short buffer[256];
    char randomString[100];

    ata_read_sector(FAT32_PARTITION_START_LBA, 1, buffer);
    copy_memory((char *) buffer, (char *) &bootSector, sizeof(Fat32BootSector));
}

unsigned int fat32_get_next_cluster(unsigned int cluster){
    unsigned int FATStartLBA = FAT32_PARTITION_START_LBA + bootSector.reservedSectorCount;
    unsigned int byteOffset = cluster * 4;
    unsigned int sectorToRead = FATStartLBA + (byteOffset/bootSector.bytesPerSector);
    unsigned int offsetInSector = byteOffset % bootSector.bytesPerSector;
    unsigned short buffer[256];

    ata_read_sector(sectorToRead, 1, buffer);

    unsigned int *buffer32 = (unsigned int *) buffer;
    unsigned int fatEntry = buffer32[offsetInSector / 4];

    return fatEntry & 0x0fffffff;
}
