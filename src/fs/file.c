#include "file.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../status.h"
#include "../memory/heap/kernel_heap.h"
#include "./fat/fat16.h"
#include "path_parser.h"
#include "../disk/disk.h"
#include "../kernel.h"

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
    fs_insert_filesystem(fat16_init());
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

FILE_MODE file_get_mode_by_string(const char* str);

int fopen(const char* file_name, const char* mode_str){
    int res = 0;
    PathRoot* path_root = path_parser_parse_path(file_name, NULL);
    if (path_root == 0) {
        res = -EINVARG;
        goto out;
    }

    if(!path_root->first){
        res = -EINVARG;
        goto out;
    }

    Disk* disk = disk_get_disk(path_root->drive_num);
    if (disk == 0) {
        res = -EIO;
        goto out;
    }
    if(!disk->fs){
        res = -EIO;
        goto out;
    }
    FILE_MODE mode = file_get_mode_by_string(mode_str);
    if (mode == FILE_MODE_INVALID) {
        res = -EINVARG;
        goto out;
    }
    void* descriptor_private_data = disk->fs->open(disk, path_root->first, mode);
    if (ISERR(descriptor_private_data)) {
        res = ERROR_I(descriptor_private_data);
        goto out;
    }
    FileDescriptor* fd = 0;
    res = file_new_descriptor(&fd);
    if (res<0) {
        goto out;
    }
    fd->fs = disk->fs;
    fd->private_data = descriptor_private_data;
    fd->disk = disk;
    res = fd->index;
out:
    if(res<0){
        res = 0;
    }
    return res;
}

FILE_MODE file_get_mode_by_string(const char* str){
    FILE_MODE mode = FILE_MODE_INVALID;
    if(strncmp(str, "r", 1) == 0){
        mode = FILE_MODE_READ;
    }else if(strncmp(str, "w", 1) == 0){
        mode = FILE_MODE_WRITE;
    }else if(strncmp(str, "a", 1) == 0){
        mode = FILE_MODE_APPEND;
    }
    return mode;
}