#ifndef HEAP_H
#define HEAP_H
#include "../../config.h"
#include <stdint.h>
#include <stddef.h>

#define HEAP_BLOCK_TABLE_ENTRY_TAKEN 1
#define HEAP_BLOCK_TABLE_ENTRY_FREE 0

#define HEAP_BLOCK_HAS_NEXT 0b10000000
#define HEAP_BLOCK_IS_FIRST  0b01000000

typedef unsigned char HEAP_BLOCK_TABLE_ENTRY;

typedef struct HeapTable{
    HEAP_BLOCK_TABLE_ENTRY* block_table_entries;
    size_t block_table_size;
} HeapTable;

typedef struct Heap{
    HeapTable* heap_table;
    void* start_address;

} Heap;

int create_heap(Heap* heap, void* heap_data_pool, void* end_of_heap, HeapTable* heap_table);

void* heap_malloc(Heap* heap, size_t size);

void heap_free(Heap* heap, void* ptr);

#endif