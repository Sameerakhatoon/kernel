#include "heap.h"
#include "../../kernel.h"
#include <stdbool.h>
#include "../../status.h"
#include "../memory.h"
#include "stdint.h"

int create_heap(Heap* heap, void* heap_data_pool, void* end_of_heap, HeapTable* heap_table);

static bool heap_validate_alignment(void* ptr);

static int heap_validate_table(void* heap_data_pool, void* end_of_heap, HeapTable* heap_table);

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
    memset(heap_table->block_table_entries, HEAP_BLOCK_IS_FREE, table_size);
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

void* heap_malloc(size_t size){
    return 0;
}

static uint32_t heap_align_value_to_upper(uint32_t value){
    if((value % OS_HEAP_BLOCK_SIZE) == 0){
        return value;
    }
    return value - (value % OS_HEAP_BLOCK_SIZE) + OS_HEAP_BLOCK_SIZE;
}

void heap_free(void* ptr){
    return;
}