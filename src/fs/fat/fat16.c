#include "fat16.h"
#include <stdint.h>
#include "../../disk/streamer.h"

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

FileSystem fat16_fs = {
    .open = fat16_open, // Placeholder for the open function
    .resolve = fat16_resolve,
};

FileSystem* fat16_init() {
    strcpy(fat16_fs.name, "FAT16");
    return &fat16_fs;
}

int fat16_resolve(Disk* disk) {
    // Check if the disk is a FAT16 disk
    // This is a placeholder implementation
    // In a real implementation, you would read the boot sector and check the filesystem type
    return 0; // Not a FAT16 filesystem
}

void *fat16_open(Disk* disk, PathPart* path, FILE_MODE mode) {
    // Open a file in the FAT16 filesystem
    // This is a placeholder implementation
    // In a real implementation, you would read the directory entries and find the file
    return 0;
}