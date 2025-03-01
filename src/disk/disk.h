#ifndef DISK_H
#define DISK_H

int disk_read_sector(int lba, int block_count, void *buffer);

#endif