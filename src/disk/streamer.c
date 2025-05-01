#include "streamer.h"
#include "../memory/heap/kernel_heap.h"
#include "../config.h"

DiskStream* diskstreamer_new(int disk_id){
    Disk* disk = disk_get_disk(disk_id);
    if(disk == NULL){
        return NULL;
    }
    DiskStream* stream = (DiskStream*)kmalloc(sizeof(DiskStream));
    stream->disk = disk;
    stream->position = 0;

    return stream;
}

int diskstreamer_seek(DiskStream* stream, int position){
    if(position < 0){
        return -1;
    }
    stream->position = position;
    return 0;
}

int diskstreamer_read(DiskStream* stream, void* buffer, int size){
    if(size < 0){
        return -1;
    }
    int sector = stream->position / OS_SECTOR_SIZE;
    int offset = stream->position % OS_SECTOR_SIZE;
    char sector_buffer[OS_SECTOR_SIZE]; //so we doen't read in buffer passed in arguments overflowing it

    int result = disk_read_block(stream->disk, sector, 1, sector_buffer);
    if(result < 0){
        goto out;
    }
    int total_to_read = size > OS_SECTOR_SIZE ? OS_SECTOR_SIZE : size;
    for(int i = 0; i < total_to_read; i++){
        ((char*)buffer)[i] = sector_buffer[offset + i];
    }
    stream->position += total_to_read;
    if(total_to_read > OS_SECTOR_SIZE){
        result = diskstreamer_read(stream, (char*)buffer, size - OS_SECTOR_SIZE);
    }

out:
    return result;
}

void diskstreamer_close(DiskStream* stream){
    kfree(stream);
}   