#ifndef DISK_H
#define DISK_H

typedef unsigned int OS_DISK_TYPE;

#define OS_DISK_TYPE_REAL 0 // Real disk

typedef struct Disk {
    OS_DISK_TYPE disk_type;
    int sector_size;
} Disk;

void disk_Search_and_init();

Disk* disk_get_disk(int disk_number);

int disk_read_block(Disk* idisk, unsigned int lba, int block_count, void* buffer);

#endif