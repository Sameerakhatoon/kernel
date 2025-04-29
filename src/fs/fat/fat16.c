#include "fat16.h"

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