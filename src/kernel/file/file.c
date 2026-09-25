#include "file.h"
#include "../../cpu/fat/fat32.h"

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
    
}