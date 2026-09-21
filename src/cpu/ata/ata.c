#include "ata.h"
#include "../ports/ports.h"

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
        do{
            status = port_byte_in(ATA_PRIMARY_STATUS);
            if(status & (ATA_STATUS_ERR | ATA_STATUS_DF)){
                return;
            }
        }while((status & ATA_STATUS_BSY) || !(status & ATA_STATUS_DRQ));
        for (i = 0; i < 256; i++){
            *buffer = port_word_in(ATA_PRIMARY_DATA);
            buffer++;
        }
        sectorCount --;
    }
}