#pragma once

// https://wiki.osdev.org/FAT#FAT_32
// https://wiki.osdev.org/User:Requimrar/FAT32

#define FAT32_PARTITION_START_LBA   2048
#define FAT32_END_OF_CHAIN          0xFFFFFFF8

#define FAT32_ATTR_READ_ONLY 0x01
#define FAT32_ATTR_HIDDEN    0x02
#define FAT32_ATTR_SYSTEM    0x04
#define FAT32_ATTR_VOLUME_ID 0x08
#define FAT32_ATTR_DIRECTORY 0x10
#define FAT32_ATTR_ARCHIVE   0x20
#define FAT32_ATTR_LFN       0x0F

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
    unsigned char   media;
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

typedef struct DIR_ENTRY Fat32DirEntry;
struct DIR_ENTRY{
    unsigned char   name[11];
    unsigned char   attr;
    unsigned char   ntReserved;
    unsigned char  createTimeTenth;
    unsigned short  createTime;
    unsigned short  createDate;
    unsigned short  lastAccess;
    unsigned short  firstClusterHigh;
    unsigned short  writeTime;
    unsigned short  writeDate;
    unsigned short  firstClusterLow;
    unsigned int    fileSize;

}__attribute__((packed));

extern Fat32BootSector bootSector;

unsigned int fat32_get_next_cluster(unsigned int cluster);
unsigned int fat32_set_next_cluster(unsigned int cluster, unsigned int value);
unsigned int fat32_cluster_to_LBA(unsigned int cluster);
unsigned int fat32_get_entry_cluster(Fat32DirEntry *entry);
unsigned int fat32_get_free_cluster(void);
unsigned int fat32_caf_cluster(void);

int fat32_find_entry(unsigned int dirCluster, const char *name, Fat32DirEntry *outEntry, unsigned int *outSector, unsigned int *outIndex);
int fat32_create_file(unsigned int dirCluster, const char *name);
int fat32_write_entry_in_dir(unsigned int cluster, Fat32DirEntry *newEntry);

void fat32_init(void);
void fat32_list_dir(unsigned int cluster);
void fat32_get_root_dir(void);
void fat32_entry_name_to_string(unsigned char rawName[11], char out[13]);
void fat32_string_to_entry_name(const char *name, unsigned char out[11]);
void fat32_ziak_cluster(unsigned int startCluster);
void fat32_make_entry(Fat32DirEntry *entry, const char *name, unsigned char attr, unsigned int cluster, unsigned int size);
void fat32_update_entry(unsigned int index, unsigned int sector, Fat32DirEntry *entry);