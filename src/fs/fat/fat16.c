#include "fat16.h"
#include <stdint.h>
#include "../../disk/streamer.h"
#include "../../memory/memory.h"
#include "../../memory/heap/kernel_heap.h"
#include "../../status.h"
#include "../../config.h"

#define OS_FAT16_SIGNATURE 0x29
#define OS_FAT16_FAT_ENTRY_SIZE 0x02
#define OS_FAT16_BAD_SECTOR 0xFF7
#define OS_FAT16_UNUSED 0x00

typedef unsigned int FAT_ITEM_TYPE;
#define FAT_ITEM_TYPE_DIRECTORY 0x00
#define FAT_ITEM_TYPE_FILE 0x01

//fat directory entry attribute bitmask
#define FAT_FILE_READ_ONLY 0x01
#define FAT_FILE_HIDDEN 0x02
#define FAT_FILE_SYSTEM 0x04
#define FAT_FILE_VOLUME_ID 0x08
#define FAT_FILE_SUBDIRECTORY 0x10
#define FAT_FILE_ARCHIVE 0x20
#define FAT_FILE_DEVICE 0x40
#define FAT_FILE_RESEVED 0x80

typedef struct __attribute__((packed)){
    uint8_t drive_number;
    uint8_t win_nt_bit;
    uint8_t signature;
    uint32_t volume_id;
    uint8_t volume_id_string[11];
    uint8_t system_id_string[8];
} FatHeaderExtended;

typedef struct __attribute__((packed)){
    uint8_t short_jmp_ins[3];
    uint8_t oem_identifier[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_copies;
    uint16_t root_dir_entries;
    uint16_t number_of_sectors;
    uint8_t media_type;
    uint16_t sectors_per_fat;
    uint16_t sectors_per_track;
    uint16_t number_of_heads;
    uint32_t hidden_setors;
    uint32_t sectors_big;
} FatHeader;

typedef struct FatH{
    FatHeader primary_header;
    union FatHE{
        FatHeaderExtended extended_header;
    } shared;
} FatH;

typedef struct  __attribute__((packed)){
    uint8_t filename[8];
    uint8_t ext[3];
    uint8_t attribute;
    uint8_t reserved;
    uint8_t creation_time_tenths_of_a_sec;
    uint16_t creation_time;
    uint16_t creation_date;
    uint16_t last_access;
    uint16_t high_16_bits_first_cluster;
    uint16_t last_mod_time;
    uint16_t last_mod_date;
    uint16_t low_16_bits_first_cluster;
    uint32_t filesize;
} FatDirectoryItem;

typedef struct FatDirectory{
    FatDirectoryItem* item;
    int total;
    int sector_pos;
    int ending_sector_pos;
} FatDirectory;

typedef struct FatItem{
    union{
        FatDirectoryItem* item;
        FatDirectory* directory;
    } Item;
    FAT_ITEM_TYPE type;
} FatItem;

typedef struct FatPrivate{
    FatH header;
    FatDirectory root_directory;

    //used to stream data clusters
    DiskStream* cluster_read_stream;
    //used to stream fat table
    DiskStream* fat_read_stream;
    //used to stream directory
    DiskStream* directory_stream;
} FatPrivate;

typedef struct FatItemDescriptor{
    FatItem* item;
    uint32_t pos;
} FatItemDescriptor;

int fat16_resolve(Disk* disk);

void *fat16_open(Disk* disk, PathPart* path, FILE_MODE mode);

FileSystem* fat16_init();

void fat16_init_private(Disk* disk, FatPrivate* fat16_private);

int fat16_get_root_directory(Disk* disk, FatPrivate* fat16_private, FatDirectory* directory);

int fat16_sector_to_absolute(Disk* disk, int sector);

int fat16_get_total_items_in_directory(Disk* disk, uint32_t directory_start_sector);

FileSystem fat16_fs = {
    .open = fat16_open, // Placeholder for the open function
    .resolve = fat16_resolve,
};

FileSystem* fat16_init() {
    strcpy(fat16_fs.name, "FAT16");
    return &fat16_fs;
}

int fat16_resolve(Disk* disk) {
    int res = 0;

    FatPrivate* fat16_private = kzalloc(sizeof(FatPrivate));
    fat16_init_private(disk, fat16_private);

    disk->fs_private = fat16_private;
    disk->fs = &fat16_fs;
    
    DiskStream* stream = diskstreamer_new(disk->id);
    if (stream == NULL) {
        res = -ENOMEM;
        goto out;
    }
    if(diskstreamer_read(stream, &fat16_private->header, sizeof(fat16_private->header)) != OS_ALL_OK) {
        res = -EIO;
        goto out;
    }
    if(fat16_private->header.shared.extended_header.signature != OS_FAT16_SIGNATURE) {
        res = -EFSNOTUS;
        goto out;
    }
    if(fat16_get_root_directory(disk, fat16_private, &fat16_private->root_directory) != OS_ALL_OK) {
        res = -EIO;
        goto out;
    }
out:
    if (stream) {
        diskstreamer_close(stream);
    }
    if (fat16_private && res < 0) {
        kfree(fat16_private);
    }
    if (res < 0){
        disk->fs_private = 0;
    }
    return res;
}

void fat16_init_private(Disk* disk, FatPrivate* fat16_private) {
    memset(fat16_private, 0, sizeof(FatPrivate));
    fat16_private->cluster_read_stream = diskstreamer_new(disk->id);
    fat16_private->fat_read_stream = diskstreamer_new(disk->id);
    fat16_private->directory_stream = diskstreamer_new(disk->id);
}

int fat16_get_root_directory(Disk* disk, FatPrivate* fat16_private, FatDirectory* directory) {
    int res = 0;

    FatHeader* primary_header = &fat16_private->header.primary_header;
    int root_dir_sector_pos = (primary_header->fat_copies * primary_header->sectors_per_fat) + primary_header->reserved_sectors;
    int root_dir_entries = primary_header->root_dir_entries;
    int root_dir_size = (root_dir_entries * sizeof(FatDirectoryItem)); 
    int total_sectors = root_dir_size/disk->sector_size;
    if (root_dir_size % disk->sector_size) {
        total_sectors++;
    }
    int total_items = fat16_get_total_items_in_directory(disk, root_dir_sector_pos);

    FatDirectoryItem* dir = kzalloc(root_dir_size);
    if(!dir){
        res = -ENOMEM;
        goto out;
    }
    DiskStream* stream = fat16_private->directory_stream;
    if(diskstreamer_seek(stream, fat16_sector_to_absolute(disk, root_dir_sector_pos)!= OS_ALL_OK)){
        res = -EIO;
        goto out;
    }

    if(diskstreamer_read(stream, dir, root_dir_size) != OS_ALL_OK){
        res = -EIO;
        goto out;
    }

    directory->item = dir;
    directory->total = total_items;
    directory->sector_pos = root_dir_sector_pos;
    directory->ending_sector_pos = root_dir_sector_pos + (root_dir_size / disk->sector_size);
    
out:
    return res;
}

int fat16_sector_to_absolute(Disk* disk, int sector) {
    // Convert a logical sector number to an absolute sector number
    return (disk->sector_size * sector);
}
int fat16_get_total_items_in_directory(Disk* disk, uint32_t directory_start_sector){
    FatDirectoryItem item;
    FatDirectoryItem empty_item;
    memset(&empty_item, 0, sizeof(empty_item));
    
    FatPrivate* fat16_private = disk->fs_private;
    int res = 0;
    int i = 0;
    int directory_start_pos = directory_start_sector * disk->sector_size;
    DiskStream* stream = fat16_private->directory_stream;
    if(diskstreamer_seek(stream, directory_start_pos) != OS_ALL_OK){
        res = -EIO;
        goto out;
    }
    while(1){
        if(diskstreamer_read(stream, &item, sizeof(item)) != OS_ALL_OK){
            res = -EIO;
            goto out;
        }
        if(item.filename[0] == 0x00){
            break; // blank record, we're done
        }
        if(item.filename[0] == 0xE5){
            continue; //item is unused
        }
        i++;
    }
    res = i;
out:
    return res;
}
void *fat16_open(Disk* disk, PathPart* path, FILE_MODE mode) {
    // Open a file in the FAT16 filesystem
    // This is a placeholder implementation
    // In a real implementation, you would read the directory entries and find the file
    return 0;
}