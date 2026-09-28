#include "ata.h"
#include "../ports/ports.h"

int wait_for_drive(unsigned char *status, int checkDrq){
    
    if(checkDrq == 0){
        do{
            *status = port_byte_in(ATA_PRIMARY_STATUS);
            if(*status & (ATA_STATUS_ERR | ATA_STATUS_DF)) {
                return 1;
            }
        }while (*status & ATA_STATUS_BSY);
        return 0;
    }
    else if(checkDrq == 1){
        do{
            *status = port_byte_in(ATA_PRIMARY_STATUS);
            if(*status & (ATA_STATUS_ERR | ATA_STATUS_DF)) {
                return 1;
            }
        }while((*status & ATA_STATUS_BSY) || !(*status & ATA_STATUS_DRQ));
        return 0;
    }
}

void ata_read_sector(unsigned int lba, unsigned char sectorCount, unsigned short *buffer){
    int i;
    unsigned char status;

    // Sur un vrai hardware, attendre le reset de BSY et DRQ du bus ~400ns
    // Ça peut être utile de le mettre en fin de lecture/écriture de secteur
    // À vérifier sur test physique
    /*for (i = 0; i < 5; i++){
        port_byte_in(ATA_PRIMARY_CONTROL);
    }*/

    do{
        status = port_byte_in(ATA_PRIMARY_STATUS);
    }while(status & ATA_STATUS_BSY);
    
    port_byte_out(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0f));
    port_byte_out(ATA_PRIMARY_SECTOR_COUNT, sectorCount);
    port_byte_out(ATA_PRIMARY_LBA_LOW, (unsigned char)(lba & 0xff));
    port_byte_out(ATA_PRIMARY_LBA_MID, (unsigned char)((lba >> 8) & 0xff));
    port_byte_out(ATA_PRIMARY_LBA_HIGH, (unsigned char)((lba >> 16)& 0xff));
    port_byte_out(ATA_PRIMARY_COMMAND, ATA_CMD_READ_SECTORS);
    

    while(sectorCount > 0){
        if(wait_for_drive(&status, 1) == 1){return;}
        for (i = 0; i < 256; i++){
            *buffer = port_word_in(ATA_PRIMARY_DATA);
            buffer++;
        }
        sectorCount --;
    }
}

void ata_write_sector(unsigned int lba, unsigned char sectorCount, unsigned short *buffer){
    int i;
    unsigned char status;

    do{
        status = port_byte_in(ATA_PRIMARY_STATUS);
    }while(status & ATA_STATUS_BSY);
    
    port_byte_out(ATA_PRIMARY_DRIVE_HEAD, 0xE0 | ((lba >> 24) & 0x0f));
    port_byte_out(ATA_PRIMARY_SECTOR_COUNT, sectorCount);
    port_byte_out(ATA_PRIMARY_LBA_LOW, (unsigned char)(lba & 0xff));
    port_byte_out(ATA_PRIMARY_LBA_MID, (unsigned char)((lba >> 8) & 0xff));
    port_byte_out(ATA_PRIMARY_LBA_HIGH, (unsigned char)((lba >> 16)& 0xff));
    port_byte_out(ATA_PRIMARY_COMMAND, ATA_CMD_WRITE_SECTORS);
    

    while(sectorCount > 0){
        if(wait_for_drive(&status, 1) == 1){return;}
        for (i = 0; i < 256; i++){
            port_word_out(ATA_PRIMARY_DATA, *buffer);
            buffer++;
        }
        sectorCount --;
    }
   
    if(wait_for_drive(&status, 0) == 1){return;} 
    port_byte_out(ATA_PRIMARY_COMMAND, ATA_CMD_CACHE_FLUSH);
    if(wait_for_drive(&status, 0) == 1){return;}
}