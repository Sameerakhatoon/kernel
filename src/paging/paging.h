#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>

#define PAGING_CACHE_DISABLE 0b00010000
#define PAGING_WRITE_THROUGH 0b00001000
#define PAGING_USER_MODE 0b00000100
#define PAGING_READ_WRITE 0b00000010
#define PAGING_PRESENT 0b00000001

#define PAGING_PAGE_SIZE 0x1000
#define PAGING_PAGE_TABLE_SIZE 1024

typedef struct Paging4GBChunk {
    uint32_t* page_directory_entry;
} Paging4GBChunk;

#endif