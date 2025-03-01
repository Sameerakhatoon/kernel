#include "paging.h"
#include "../memory/heap/kernel_heap.h"
#include <stdbool.h>
#include "../status.h"


Paging4GBChunk* paging_new_4gb(uint8_t flags);

uint32_t* paging_chunk_get_page_directory(Paging4GBChunk* chunk);

void paging_switch(uint32_t* page_directory);

void paging_load_directory(uint32_t* page_directory);


int paging_get_indices(uint32_t virtual_address, uint32_t* directory_index, uint32_t* table_index);

bool paging_is_aligned(uint32_t address);

int paging_set(uint32_t* directory, void* virtual_address, uint32_t value);


static uint32_t* current_directory = 0;

// Initialize paging, this chunk will be linear, as if no paging was enabled, the other functions will be used to map the pages to other locations
Paging4GBChunk* paging_new_4gb(uint8_t flags) {
    uint32_t* page_directory = kzalloc(sizeof(uint32_t) * PAGING_PAGE_DIRECTORY_SIZE);
    int offset = 0;
    for(int i = 0; i < PAGING_PAGE_DIRECTORY_SIZE; i++) {
        uint32_t* page_table_entry = kzalloc(sizeof(uint32_t) * PAGING_PAGE_TABLE_SIZE);
        for(int j = 0; j < PAGING_PAGE_TABLE_SIZE; j++) {
            page_table_entry[j] = (offset + (j * PAGING_PAGE_SIZE)) | flags;
        }
        offset += PAGING_PAGE_TABLE_SIZE * PAGING_PAGE_SIZE;
        page_directory[i] = ((uint32_t)page_table_entry) | flags | PAGING_READ_WRITE;
    }
    Paging4GBChunk* chunk = kzalloc(sizeof(Paging4GBChunk));
    chunk->page_directory_entry = page_directory;
    return chunk;
}

uint32_t* paging_chunk_get_page_directory(Paging4GBChunk* chunk) {
    return chunk->page_directory_entry;
}

void paging_switch(uint32_t* page_directory) {
    paging_load_directory(page_directory);
    current_directory = page_directory;
}

int paging_get_indices(uint32_t virtual_address, uint32_t* directory_index, uint32_t* table_index) {
    int result = 0;
    if(!paging_is_aligned(virtual_address)) {
        result = -EINVARG;
        goto out;
    }
    *directory_index = ((uint32_t)virtual_address / (PAGING_PAGE_SIZE * PAGING_PAGE_TABLE_SIZE)) % PAGING_PAGE_DIRECTORY_SIZE;
    *table_index = ((uint32_t)virtual_address / PAGING_PAGE_SIZE) % PAGING_PAGE_TABLE_SIZE;
out:
    return result;
}

bool paging_is_aligned(uint32_t address) {
    return ((uint32_t)address % PAGING_PAGE_SIZE) == 0;
}

int paging_set(uint32_t* directory, void* virtual_address, uint32_t value) {
    if(!paging_is_aligned((uint32_t) virtual_address)) {
        return -EINVARG;
    }
    uint32_t directory_index = 0;
    uint32_t table_index = 0;
    int result = paging_get_indices((uint32_t)virtual_address, &directory_index, &table_index);
    if(result < 0) {
        return result;
    }
    uint32_t entry = directory[directory_index];
    uint32_t* table = (uint32_t*)(entry & 0xFFFFF000); // Get the table address, as 4096 byte aligned, the last 12 bits are 0, so we can use them to get the address
    table[table_index] = value;
    return 0;
}