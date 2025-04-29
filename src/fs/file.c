#include "file.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../status.h"
#include "../memory/heap/kernel_heap.h"


FileSystem* filesystems[OS_MAX_FILESYSTEMS];
FileDescriptor* file_descriptors[OS_MAX_FILEDESCRIPTORS];

static FileSystem** fs_get_free_filesystem() {
    for (int i = 0; i < OS_MAX_FILESYSTEMS; i++) {
        if (filesystems[i] == 0) {
            return &filesystems[i];
        }
    }
    return 0;
}

void fs_insert_filesystem(FileSystem* fs) {
    FileSystem** free_fs = fs_get_free_filesystem();
    if (free_fs == 0) {
        while(1){

        }
    }
    *free_fs = fs;
}

static void fs_static_load(){
    // fs_insert_filesystem(fat16_init());
}

void fs_load(){
    memset(filesystems, 0, sizeof(filesystems));
    fs_static_load();
}

void fs_init() {
    memset(file_descriptors, 0, sizeof(file_descriptors));
    fs_load();
}

static int file_new_descriptor(FileDescriptor** fd_out){
    int res = -ENOMEM;
    for (int i = 0; i < OS_MAX_FILEDESCRIPTORS; i++) {
        if (file_descriptors[i] == 0) {
            FileDescriptor* fd = kzalloc(sizeof(FileDescriptor));
            //descriptors starts at 1
            fd->index = i + 1;
            file_descriptors[i] = fd;
            *fd_out = fd;
            res = 0;
            break;
        }
    }
    return res;
}

static FileDescriptor* file_get_descriptor(int fd) {
    if (fd <=0 || fd >= OS_MAX_FILEDESCRIPTORS) {
        return 0;
    }
    int index = fd - 1; //since descriptors start at 1
    return file_descriptors[index];
}

FileSystem* fs_resolve(Disk* disk) {
    for (int i = 0; i < OS_MAX_FILESYSTEMS; i++) {
        if (filesystems[i] != 0) {
            if (filesystems[i]->resolve(disk) == 0) {
                return filesystems[i];
            }
        }
    }
    return 0;
}

int fopen(const char* file_name, const char* mode){
    return -EIO;
}