#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include <stddef.h>

#define PAGING_CACHE_DISABLE 0b00010000
#define PAGING_WRITE_THROUGH 0b00001000
#define PAGING_USER_MODE 0b00000100
#define PAGING_READ_WRITE 0b00000010
#define PAGING_PRESENT 0b00000001

#define PAGING_PAGE_SIZE 0x1000
#define PAGING_PAGE_TABLE_SIZE 1024
#define PAGING_PAGE_DIRECTORY_SIZE 1024

typedef struct Paging4GBChunk {
    uint32_t* page_directory_entry; // this is the page directory, which is an array of page table entries
} Paging4GBChunk;

Paging4GBChunk* paging_new_4gb(uint8_t flags);

uint32_t* paging_chunk_get_page_directory(Paging4GBChunk* chunk);

void paging_switch(uint32_t* page_directory);

void paging_load_directory(uint32_t* page_directory);

void enable_paging();

int paging_set(uint32_t* directory, void* virtual_address, uint32_t value);

#endif