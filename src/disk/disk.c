#include "../io/io.h"
#include "disk.h"
#include "../memory/memory.h"
#include "../config.h"
#include "../status.h"

int disk_read_sector(int lba, int block_count, void *buffer);


Disk disk;

void disk_Search_and_init();

Disk* disk_get_disk(int disk_number);

int disk_read_block(Disk* idisk, unsigned int lba, int block_count, void* buffer);

int disk_read_sector(int lba, int block_count, void *buffer) {

    out_byte(0x1F6, (lba >> 24) | 0xE0); // Selecting the master drive, and setting the highest 4 bits of the LBA
    out_byte(0x1F2, block_count); // Setting the number of sectors to read
    out_byte(0x1F3, (unsigned char)(lba & 0xff)); // Setting the LBA, first 8 bits
    out_byte(0x1F4, (unsigned char)(lba >> 8)); // Setting the LBA, next 8 bits
    out_byte(0x1F5, (unsigned char)(lba >> 16)); // Setting the LBA, next 8 bits
    out_byte(0x1F7, 0x20); // Sending the read command to the drive

    unsigned short* buffer_ptr = (unsigned short*)buffer;
    for (int i = 0; i < block_count; i++) {
        // Waiting for the drive to be ready
        while(!(in_byte(0x1F7) & 0x08));
        // Reading the sector
        for (int j = 0; j < 256; j++) {
            buffer_ptr[j] = in_word(0x1F0);
        }
    }

    return 0;
}

// Disk disk;

void disk_Search_and_init(){
    memset(&disk, 0, sizeof(Disk));
    disk.disk_type = OS_DISK_TYPE_REAL;
    disk.sector_size = OS_SECTOR_SIZE;
    disk.fs = fs_resolve(&disk);
}

Disk* disk_get_disk(int disk_number) {
    if(disk_number == 0) {
        return &disk;
    }
    return 0;
}

int disk_read_block(Disk* idisk, unsigned int lba, int block_count, void* buffer) {
    if(idisk != &disk) {
        return -EIO;
    }
    return disk_read_sector(lba, block_count, buffer);
}