#ifndef DISK_STREAMER_H
#define DISK_STREAMER_H

#include "disk.h"

typedef struct DiskStream{
    int position; //byte position currently seeking to in the disk 
    Disk* disk;
} DiskStream;

DiskStream* diskstreamer_new(int disk_id);
int diskstreamer_seek(DiskStream* stream, int position);

int diskstreamer_read(DiskStream* stream, void* buffer, int size);
void diskstreamer_close(DiskStream* stream);

#endif