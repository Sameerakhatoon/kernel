#include "../io/io.h"
#include "disk.h"

int disk_read_sector(int lba, int block_count, void *buffer);

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