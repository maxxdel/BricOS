#pragma once

#define MAX_FILES_OPEN 8

typedef struct FILE_HANDLE Fat32File;
struct FILE_HANDLE{
    unsigned int    startCluster;
    unsigned int    currentCluster;
    unsigned int    fileSize;
    unsigned int    position;
    unsigned int    dirEntrySector;
    unsigned int    dirEntryIndex;
    int             isOpen;
};

static Fat32File openFiles[MAX_FILES_OPEN];

int file_open(unsigned int dirCluster, const char *name);
unsigned int file_read(int fileDescriptor, char *buffer, unsigned int size);
unsigned int file_write(int fileDescriptor, char *buffer, unsigned int size);
void file_close(int fileDescriptor);