#ifndef FILE_H
#define FILE_H

#include "../disk/disk.h"
#include "path_parser.h"

typedef unsigned int FILE_SEEK_MODE;

enum{
    SEEK_SET = 0,
    SEEK_CUR = 1,
    SEEK_END = 2
};

typedef unsigned int FILE_MODE;
enum{
    FILE_MODE_READ = 0,
    FILE_MODE_WRITE = 1,
    FILE_MODE_APPEND = 2,
    FILE_MODE_INVALID = 3
};


typedef struct Disk Disk;
typedef void*(*FS_OPEN_FUNCTION)(Disk* disk, PathPart* path, FILE_MODE mode);
typedef int(*FS_RESOLVE_FUNCTION)(Disk* disk);


typedef struct FileSystem FileSystem;

struct FileSystem{
    char name[20];
    FS_OPEN_FUNCTION open;
    FS_RESOLVE_FUNCTION resolve;
};

typedef struct FileDescriptor{
    int index; // index in the file descriptor table
    FileSystem* fs; // file system
    void* private_data; // pvt data for internal descriptor
    Disk* disk; // disk file decriptor should be used on
} FileDescriptor;

void fs_init();
int fopen(const char* file_name, const char* mode_str);
void fs_insert_filesystem(FileSystem* fs);
FileSystem* fs_resolve(Disk* disk);

#endif