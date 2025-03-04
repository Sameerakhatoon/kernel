#include "path_parser.h"
#include "../kernel.h"
#include "../string/string.h"
#include "../memory/memory.h"
#include "../status.h"
#include "../memory/heap/kernel_heap.h"

static int path_parser_path_valid_format(const char *path);
static int path_parser_get_drive_num(const char** path);
static PathRoot* path_parser_create_root(int drive_num);
static const char* path_parser_get_path_part(const char** path);
PathPart* path_parser_create_path_part(PathPart* last, const char** path);
void path_parser_free_path(PathRoot* root);
PathRoot* path_parser_parse_path(const char* path, const char* current_dir);







static int path_parser_path_valid_format(const char *path) {
    int len = strnlen(path, OS_MAX_PATH_LENGTH);
    return len >= 3 && is_digit(path[0]) && memcmp((void*)&path[1], ":/", 2) == 0;
}

static int path_parser_get_drive_num(const char** path) {
    if(!path_parser_path_valid_format(*path)) {
        return -EBADPATH;
    }
    int drive_num = char_to_numeric_digit((*path)[0]);
    *path += 3; // Skip drive number and ":/"
    return drive_num;
}

static PathRoot* path_parser_create_root(int drive_num){
    PathRoot* root = kzalloc(sizeof(PathRoot));
    if(root == NULL){
        return NULL;
    }
    root->drive_num = drive_num;
    root->first = 0;
    return root;
}

static const char* path_parser_get_path_part(const char** path){
    if (**path == '\0') {
        return NULL;
    }
    char* result_path_part = kzalloc(OS_MAX_PATH_LENGTH);
    if(result_path_part == NULL){
        return NULL;
    }
    int i = 0;
    while(**path != '/' && **path != '\0'){
        result_path_part[i++] = **path;
        (*path)++;
    }
    if(**path == '/'){
        (*path)++;
    }
    return result_path_part;
}

PathPart* path_parser_create_path_part(PathPart* last, const char** path){
    const char* path_part = path_parser_get_path_part(path);
    if(!path_part){
        return NULL;
    }
    PathPart* part = kzalloc(sizeof(PathPart));
    if(part == NULL){
        return NULL;
    }
    part->name = path_part;
    part->next = NULL;
    if(last != NULL){
        last->next = part;
    }
    return part;
}

void path_parser_free_path(PathRoot* root){
    PathPart* current = root->first;
    while(current != NULL){
        PathPart* next = current->next;
        kfree(current);
        current = next;
    }
    kfree(root);
}

PathRoot* path_parser_parse_path(const char* path, const char* current_dir){
    int drive_num = 0;
    const char* path_copy = path;
    PathRoot* root = NULL;
    if(strlen(path) > OS_MAX_PATH_LENGTH){
        goto out;
    }
    drive_num = path_parser_get_drive_num(&path_copy);
    if(drive_num < 0){
        goto out;
    }
    root = path_parser_create_root(drive_num);
    if(root == NULL){
        goto out;
    }
    PathPart* first_part = path_parser_create_path_part(NULL, &path_copy);
    if(first_part == NULL){
        goto out;
    }
    root->first = first_part;
    PathPart* last_part = path_parser_create_path_part(first_part, &path_copy);
    while(last_part){
        PathPart* next_part = path_parser_create_path_part(last_part, &path_copy);
        if (!next_part) {
            break;
        }
        last_part = next_part;    }
out:
    return root;
}