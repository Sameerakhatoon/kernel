#ifndef DISK_H
#define DISK_H
#include "../fs/file.h"

typedef unsigned int OS_DISK_TYPE;

#define OS_DISK_TYPE_REAL 0 // Real disk

typedef struct Disk Disk;

typedef struct FileSystem FileSystem;

struct Disk {
    OS_DISK_TYPE disk_type;
    int sector_size;
    FileSystem* fs;
};

void disk_Search_and_init();

Disk* disk_get_disk(int disk_number);

int disk_read_block(Disk* idisk, unsigned int lba, int block_count, void* buffer);

#endif