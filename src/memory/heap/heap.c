#include "heap.h"
#include "../../kernel.h"
#include <stdbool.h>
#include "../../status.h"
#include "../memory.h"
#include "stdint.h"

int create_heap(Heap* heap, void* heap_data_pool, void* end_of_heap, HeapTable* heap_table);

static bool heap_validate_alignment(void* ptr);

static int heap_validate_table(void* heap_data_pool, void* end_of_heap, HeapTable* heap_table);

void* heap_malloc(Heap* heap, size_t size);

static uint32_t heap_align_value_to_upper(uint32_t value);

void heap_free(Heap* heap, void* ptr);

void* heap_malloc_blocks(Heap* heap, uint32_t total_blocks);

int heap_get_start_block(Heap* heap, uint32_t total_blocks);

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry);

void* heap_block_to_address(Heap* heap, int block);

void heap_mark_blocks_taken(Heap* heap, int start_block, uint32_t total_blocks);

void heap_mark_blocks_free(Heap* heap, int start_block);

static int heap_address_to_block(Heap* heap, void* address);





int create_heap(Heap* heap, void* heap_data_pool, void* end_of_heap, HeapTable* heap_table){
    int result = 0;
    if(!heap_validate_alignment(heap_data_pool) || !heap_validate_alignment(end_of_heap)){
        result = -EINVARG;
        goto out;
    }
    memset(heap, 0, sizeof(Heap));
    heap->start_address = heap_data_pool;
    heap->heap_table = heap_table;

    result = heap_validate_table(heap_data_pool, end_of_heap, heap_table);
    if(result<0){
        goto out;
    }
    size_t table_size = sizeof(HEAP_BLOCK_TABLE_ENTRY)*heap_table->block_table_size;
    memset(heap_table->block_table_entries, HEAP_BLOCK_TABLE_ENTRY_FREE, table_size);
out:
    return result;
}

static bool heap_validate_alignment(void* ptr){
    return ((unsigned int)ptr % OS_HEAP_BLOCK_SIZE) == 0;
}

static int heap_validate_table(void* heap_data_pool, void* end_of_heap, HeapTable* heap_table){
    int result = 0;
    size_t table_size = (size_t)(end_of_heap - heap_data_pool);
    size_t table_blocks = table_size / OS_HEAP_BLOCK_SIZE;
    if(heap_table->block_table_size != table_blocks){
        result = -EINVARG;
        goto out;
    }
out:
    return result;
}

void* heap_malloc(Heap* heap, size_t size){
    size_t aligned_size = heap_align_value_to_upper(size);
    uint32_t total_blocks = aligned_size / OS_HEAP_BLOCK_SIZE;
    return heap_malloc_blocks(heap, total_blocks);
}

static uint32_t heap_align_value_to_upper(uint32_t value){
    if((value % OS_HEAP_BLOCK_SIZE) == 0){
        return value;
    }
    value = value - (value % OS_HEAP_BLOCK_SIZE);
    value += OS_HEAP_BLOCK_SIZE;
    return value;
}

void heap_free(Heap* heap, void* ptr){
    heap_mark_blocks_free(heap, heap_address_to_block(heap, ptr));    
}

void* heap_malloc_blocks(Heap* heap, uint32_t total_blocks){
    void* address = 0;
    int start_block = heap_get_start_block(heap, total_blocks);
    if(start_block < 0){
        goto out;
    }
    address = heap_block_to_address(heap, start_block);
    //mark blocks as used
    heap_mark_blocks_taken(heap, start_block, total_blocks);
out:
    return address;
}

int heap_get_start_block(Heap* heap, uint32_t total_blocks){
    HeapTable* heap_table = heap->heap_table;
    int current_block = 0;
    int start_block = -1;
    for(size_t i = 0; i < heap_table->block_table_size; i++){
        if(heap_get_entry_type(heap_table->block_table_entries[i])!=HEAP_BLOCK_TABLE_ENTRY_FREE){
            current_block = 0;
            start_block = -1;
            continue;
        }
        if(start_block == -1){
            start_block = i;
        }
        current_block++;
        if(current_block == total_blocks){
            break;
        }
    }
    if(start_block == -1){
        return -ENOMEM;
    }
    return start_block;
}

static int heap_get_entry_type(HEAP_BLOCK_TABLE_ENTRY entry){
    return entry & 0x0f;
}

void* heap_block_to_address(Heap* heap, int block){
    return heap->start_address + (block * OS_HEAP_BLOCK_SIZE);
}

void heap_mark_blocks_taken(Heap* heap, int start_block, uint32_t total_blocks){
    int end_block = start_block + total_blocks - 1;
    HEAP_BLOCK_TABLE_ENTRY entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN | HEAP_BLOCK_IS_FIRST;
    if(total_blocks>1){
        entry |= HEAP_BLOCK_HAS_NEXT;
    }
    for(int i = start_block; i <= end_block; i++){
        heap->heap_table->block_table_entries[i] = entry;
        entry = HEAP_BLOCK_TABLE_ENTRY_TAKEN;
        if(i != end_block - 1){
            entry |= HEAP_BLOCK_HAS_NEXT;
        }
    }
}

void heap_mark_blocks_free(Heap* heap, int start_block){
    HeapTable* table = heap->heap_table;
    for(size_t i = start_block; i < table->block_table_size; i++){
        HEAP_BLOCK_TABLE_ENTRY entry = table->block_table_entries[i];
        table->block_table_entries[i] = HEAP_BLOCK_TABLE_ENTRY_FREE;
        if((entry & HEAP_BLOCK_HAS_NEXT) == 0){
            break;
        }
    }
}

static int heap_address_to_block(Heap* heap, void* address){
    return ((int)(address - heap->start_address)) / OS_HEAP_BLOCK_SIZE;
}