#include "file.h"
#include "../../cpu/fat/fat32.h"
#include "../../cpu/ata/ata.h"
#include "../../memory/mem.h"

int file_open(unsigned int dirCluster, const char *name){
    Fat32DirEntry entry;
    unsigned int sector, index;
    if(!(fat32_find_entry(dirCluster, name, &entry, &sector, &index))){return -1;}

    for(int i = 0; i < MAX_FILES_OPEN; i++){
        if(!(openFiles[i].isOpen)){
            openFiles[i].startCluster       = fat32_get_entry_cluster(&entry);
            openFiles[i].currentCluster     = openFiles[i].startCluster;
            openFiles[i].fileSize           = entry.fileSize;
            openFiles[i].position           = 0;
            openFiles[i].dirEntrySector     = sector;
            openFiles[i].dirEntryIndex      = index;
            openFiles[i].isOpen             = 1;
            return i;
        }
    }
    return -1;
}

unsigned int file_read(int fileDescriptor, char *buffer, unsigned int size){
        if(fileDescriptor < 0 || fileDescriptor >= MAX_FILES_OPEN || !(openFiles[fileDescriptor].isOpen)){return 0;}

        Fat32File *file = &openFiles[fileDescriptor];

        if(file->position >= file->fileSize){return 0;}
        
        if(file->position + size > file->fileSize){
            size = file->fileSize - file->position;
        }

        unsigned int clusterSizeInBytes = bootSector.sectorsPerCluster * bootSector.bytesPerSector;
        unsigned int bytes = 0;

        while(bytes < size){
            unsigned int offsetInCluster    = file->position % clusterSizeInBytes;
            unsigned int sectorInCluster    = offsetInCluster / bootSector.bytesPerSector;
            unsigned int offsetInSector     = offsetInCluster % bootSector.bytesPerSector;
            unsigned short sectorBuffer[256]; 

            ata_read_sector(fat32_cluster_to_LBA(file->currentCluster) + sectorInCluster, 1, sectorBuffer);

            unsigned char *sectorBytes = (unsigned char *) sectorBuffer;
            unsigned int copy = bootSector.bytesPerSector - offsetInSector;

            if(copy > size - bytes){    
                copy = size - bytes;
            }

            copy_memory((char *)(sectorBytes + offsetInSector), (char *)(buffer + bytes), copy);
            
            bytes += copy;
            file->position += copy;

            if(offsetInSector + copy == bootSector.bytesPerSector && (sectorInCluster + 1) == bootSector.sectorsPerCluster){
                file->currentCluster =  fat32_get_next_cluster(file->currentCluster);
            }
        }
    return bytes;
}

unsigned int file_write(int fileDescriptor, char *buffer, unsigned int size){
        if(fileDescriptor < 0 || fileDescriptor >= MAX_FILES_OPEN || !(openFiles[fileDescriptor].isOpen)){return 0;}

        Fat32File *file = &openFiles[fileDescriptor];
        
        if(file->startCluster == 0){
            unsigned int cluster = fat32_caf_cluster();
            if(cluster == 0){return 0;}

            file->startCluster      = cluster;
            file->currentCluster    = cluster;
        }

        unsigned int clusterSizeInBytes = bootSector.sectorsPerCluster * bootSector.bytesPerSector;
        unsigned int bytes = 0;

        while(bytes < size){
            unsigned int offsetInCluster    = file->position % clusterSizeInBytes;
            unsigned int sectorInCluster    = offsetInCluster / bootSector.bytesPerSector;
            unsigned int offsetInSector     = offsetInCluster % bootSector.bytesPerSector;
            unsigned short sectorBuffer[256]; 

            ata_read_sector(fat32_cluster_to_LBA(file->currentCluster) + sectorInCluster, 1, sectorBuffer);

            unsigned char *sectorBytes = (unsigned char *) sectorBuffer;
            unsigned int copy = bootSector.bytesPerSector - offsetInSector;

            if(copy > size - bytes){    
                copy = size - bytes;
            }

            copy_memory((char *)(buffer + bytes), (char *)(sectorBytes + offsetInSector), copy);
            ata_write_sector(fat32_cluster_to_LBA(file->currentCluster) + sectorInCluster, 1, sectorBuffer);

            bytes += copy;
            file->position += copy;

            if(file->position >= file->fileSize){
                file->fileSize = file->position;
            }

            if(offsetInSector + copy == bootSector.bytesPerSector && (sectorInCluster + 1) == bootSector.sectorsPerCluster && bytes < size){
                unsigned int nextCluster =  fat32_get_next_cluster(file->currentCluster);
                if(nextCluster >= FAT32_END_OF_CHAIN){
                    nextCluster = fat32_caf_cluster();

                    if(nextCluster == 0){return 0;}

                    fat32_set_next_cluster(file->currentCluster, nextCluster);
                }
            file->currentCluster = nextCluster;
            }
        }
    return bytes;
}

void file_close(int fileDescriptor){
    if(fileDescriptor < 0 || fileDescriptor >= MAX_FILES_OPEN || !(openFiles[fileDescriptor].isOpen)){return;}

    Fat32File *file = &openFiles[fileDescriptor];

    Fat32DirEntry entry;
    unsigned short buffer[256];

    ata_read_sector(file->dirEntrySector, 1, buffer);
    copy_memory((char *) (&((Fat32DirEntry *) buffer)[file->dirEntryIndex]), (char *) &entry, sizeof(Fat32DirEntry));

    entry.fileSize          = file->fileSize;
    entry.firstClusterHigh  = ((file->startCluster >> 16) & 0xffff);
    entry.firstClusterLow   = ((file->startCluster) & 0xffff);

    fat32_update_entry(file->dirEntryIndex, file->dirEntrySector, &entry);
    file->isOpen = 0;
}