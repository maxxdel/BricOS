#pragma once

// https://wiki.osdev.org/FAT#FAT_32

#define FAT32_PARTITION_START_LBA   2048
#define FAT32_END_OF_CHAIN          0xFFFFFFF8

typedef struct BOOT_SECTOR Fat32BootSector;
struct BOOT_SECTOR{
    unsigned char   jumpBoot[3];
    unsigned char   oemName[8];
    unsigned short  bytesPerSector;
    unsigned char   sectorsPerCluster;
    unsigned short  reservedSectorCount;
    unsigned char   numFat;
    unsigned short  rootEntryCount;
    unsigned short  totalSectors16;
    unsigned short  fatSize16;
    unsigned short  sectorsPerTrack;
    unsigned short  numHeads;
    unsigned int    hiddenSectors;
    unsigned int    totalSectors32;
    unsigned int    fatSize32;
    unsigned short  extFlags;
    unsigned short  fsVersion;
    unsigned int    rootCluster;
    unsigned short  fsInfo;
    unsigned short  backupBootSector;
    unsigned char   reserved[12];
    unsigned char   driveNumber;
    unsigned char   reserved1;
    unsigned char   bootSignature;
    unsigned int    volumeID;
    unsigned char   volumeLabel[11];
    unsigned char   fsType[8];

}__attribute__((packed));

void fat32_init(void);
unsigned int fat32_get_next_cluster(unsigned int cluster);