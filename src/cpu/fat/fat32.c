#include "fat32.h"
#include "../ata/ata.h"
#include "../../drivers/screen/screen.h"
#include "../../kernel/utils/utils.h"
#include "../../memory/mem.h"

Fat32BootSector bootSector;

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

unsigned int fat32_set_next_cluster(unsigned int cluster, unsigned int value){
    unsigned int FATStartLBA = FAT32_PARTITION_START_LBA + bootSector.reservedSectorCount;
    unsigned int byteOffset = cluster * 4;
    unsigned int sectorToRead = FATStartLBA + (byteOffset/bootSector.bytesPerSector);
    unsigned int offsetInSector = byteOffset % bootSector.bytesPerSector;
    unsigned short buffer[256];

    ata_read_sector(sectorToRead, 1, buffer);

    unsigned int *buffer32 = (unsigned int *) buffer;
    buffer32[offsetInSector / 4] = value & 0x0fffffff;

    ata_write_sector(sectorToRead, 1, buffer);
}

static unsigned int fat32_cluster_to_LBA(unsigned int cluster){
    unsigned int firstDataSector = FAT32_PARTITION_START_LBA + bootSector.reservedSectorCount + (bootSector.numFat * bootSector.fatSize32);
    return firstDataSector + (cluster - 2) * bootSector.sectorsPerCluster;
}

void fat32_list_dir(unsigned int cluster){
    unsigned short buffer[256];
    unsigned int clusterCount = cluster;

    while(clusterCount < FAT32_END_OF_CHAIN){
        unsigned int lba = fat32_cluster_to_LBA(clusterCount);

        for (int i = 0; i < bootSector.sectorsPerCluster; i++){
            ata_read_sector(lba + i, 1, buffer);
            Fat32DirEntry *entries = (Fat32DirEntry *) buffer;

            for (int j = 0; j < 16; j++){
                Fat32DirEntry *entry = &entries[j];
                if(entry->name[0] == 0x00){return;}
                if(entry->name[0] == 0xe5){continue;}
                if(entry->attr == FAT32_ATTR_LFN){continue;}
                if(entry->attr & FAT32_ATTR_VOLUME_ID){continue;}
                
                char name [13];
                fat32_entry_name_to_string(entry->name, name);
                printf(name);
                print_char('\n');
            }

        }
        clusterCount = fat32_get_next_cluster(clusterCount);
    }
}

void fat32_get_root_dir(){
    fat32_list_dir(bootSector.rootCluster);
}

void fat32_entry_name_to_string(unsigned char rawName[11], char out[13]){
    int pos = 0;
    
    for(int i = 0; i < 8 && rawName[i] != ' '; i++){ 
        out[pos++] = rawName[i];
    }
    
    if(rawName[8] != ' '){
        out[pos++] = '.';
        for(int i = 8; i < 11 && rawName[i] != ' '; i++) out[pos++] = rawName[i];
    }

    out[pos] = '\0';
}

void fat32_string_to_entry_name(const char *name, unsigned char out[11]){
    int i = 0, j = 0;
    for(j = 0; j < 11; j++) out[j] = ' ';

    j = 0;
    while(name[i] != '\0' && name[i] != '.' && j < 8) out[j++] = to_upper(name[i++]);
    while(name[i] != '\0' && name[i] != '.') i++;
    if(name[i] == '.'){
        i++;
        j = 8;
        while(name[i] != '\0' && j < 11) out[j++] = to_upper(name[i++]);
    }
}

int fat32_find_entry(unsigned int dirCluster, const char *name, Fat32DirEntry *outEntry){
    unsigned char entryName[11];
    unsigned short buffer[256];
    unsigned int clusterCount = dirCluster;

    fat32_string_to_entry_name(name, entryName);

    while(clusterCount < FAT32_END_OF_CHAIN){
        unsigned int lba = fat32_cluster_to_LBA(clusterCount);
        for(int i = 0; i< bootSector.sectorsPerCluster; i++){
            ata_read_sector((lba + i), 1, buffer);
            Fat32DirEntry *entries = (Fat32DirEntry *) buffer;
            
            for(int j = 0; j < 16; j ++){
                Fat32DirEntry *entry = &entries[j];
                if(entry->name[0] == 0x00){return 0;}
                if(entry->name[0] == 0xe5){continue;}
                if(entry->attr == FAT32_ATTR_LFN){continue;}
                if(entry->attr & FAT32_ATTR_VOLUME_ID){continue;}
                
                bool match = true;

                for(int k = 0; k < 11; k++){
                    if(entry->name[k] != entryName[k]){
                        match = false;
                        break;
                    }
                }
                if(match){
                    copy_memory((char *) entry, (char *)outEntry, sizeof(Fat32DirEntry));
                    return 1;
                }
            }
        }
        clusterCount = fat32_get_next_cluster(clusterCount);
    }
    return 0;
}

unsigned int fat32_get_entry_cluster(Fat32DirEntry *entry){
    return ((unsigned int) entry->firstClusterHigh << 16) | entry->firstClusterLow;
}

unsigned int fat32_get_free_cluster(){
    unsigned int firstDataSector = FAT32_PARTITION_START_LBA + bootSector.reservedSectorCount + (bootSector.numFat * bootSector.fatSize32);
    unsigned int totalCLusters = (bootSector.totalSectors32 - firstDataSector + FAT32_PARTITION_START_LBA) / bootSector.sectorsPerCluster;
    
    for(int i = 2; i < totalCLusters; i++){
        if(fat32_get_next_cluster(i) ==0){
            return i;
        }
    }
    return 0;
}

unsigned int fat32_caf_cluster(){
    unsigned int cluster = fat32_get_free_cluster();
    if(cluster != 0){
        fat32_set_next_cluster(cluster, FAT32_END_OF_CHAIN);
    }
    return cluster;
}

void fat32_ziak_cluster(unsigned int startCluster){
    unsigned int currentCluster = startCluster;
    while(currentCluster < FAT32_END_OF_CHAIN && currentCluster != 0){
        unsigned int nextCluster = fat32_get_next_cluster(currentCluster);
        fat32_set_next_cluster(currentCluster, 0);
        currentCluster = nextCluster;
    }
}

int fat32_write_entry_in_dir(unsigned int cluster, Fat32DirEntry *newEntry){
    unsigned short buffer[256];
    unsigned int clusterCount = cluster;
    unsigned int lastCluster = cluster;

    while(1){
        unsigned int lba = fat32_cluster_to_LBA(clusterCount);

        for (int i = 0; i < bootSector.sectorsPerCluster; i++){
            ata_read_sector((lba + i), 1, buffer);
            Fat32DirEntry *entries = (Fat32DirEntry *) buffer;

            for (int j = 0; j < 16; j++){
                unsigned char first = entries[j].name[0];
                
                if(first == 0x00 || first == 0xe5){
                    copy_memory((char *)newEntry, (char *) &entries[j], sizeof(Fat32DirEntry));
                    ata_write_sector((lba + i), 1, buffer);
                    return 1;
                }
            }

        }
        lastCluster = clusterCount;
        unsigned int nextCluster = fat32_get_next_cluster(clusterCount);

        if(nextCluster >= FAT32_END_OF_CHAIN){
            unsigned int newCluster = fat32_get_free_cluster();
            if (newCluster == 0){return 0;}

            fat32_set_next_cluster(lastCluster, newCluster);

            unsigned short zero[256] = {0};
            unsigned int newLBA = fat32_cluster_to_LBA(newCluster);

            for(int k = 0; k < bootSector.sectorsPerCluster; k++){
                ata_write_sector((newLBA + k), 1, zero);

                clusterCount = newCluster;
            }
            clusterCount = newCluster;
        }
        else {
            clusterCount = nextCluster;
        }
    }
}

void fat32_make_entry(Fat32DirEntry *entry, const char *name, unsigned char attr, unsigned int cluster, unsigned int size){
    fat32_string_to_entry_name(name, entry->name);
    entry->attr                 = attr;
    entry->ntReserved           = 0;
    entry->createTimeTenth      = 0;
    entry->createTime           = 0;
    entry->createDate           = 0;
    entry->lastAccess           = 0;
    entry->firstClusterHigh     = (cluster >> 16) & 0xffff;
    entry->writeTime            = 0;
    entry->writeDate            = 0;
    entry->firstClusterLow      = cluster & 0xffff;
    entry->fileSize             = size;
}

int fat32_create_file(unsigned int dirCluster, const char *name){
    Fat32DirEntry checkEntry;
    
    if(fat32_find_entry(dirCluster, name, &checkEntry)){return 0;}

    unsigned int cluster = fat32_caf_cluster();
    
    if(cluster == 0){return 0;}

    Fat32DirEntry entry;
    fat32_make_entry(&entry, name, FAT32_ATTR_ARCHIVE, cluster, 0);

    return fat32_write_entry_in_dir(dirCluster, &entry);
}