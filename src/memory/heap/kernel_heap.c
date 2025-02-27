#include "kernel_heap.h"
#include "heap.h"
#include "../../config.h"
#include "../../kernel.h"

Heap kernel_heap;
HeapTable kernel_heap_table;

void init_kernel_heap(){
    int total_table_entries = (OS_HEAP_SIZE_BYTES / OS_HEAP_BLOCK_SIZE);
    kernel_heap_table.block_table_entries = (HEAP_BLOCK_TABLE_ENTRY*)OS_HEAP_TABLE_ADDRESS;
    kernel_heap_table.block_table_size = total_table_entries;
    void* end_add = (void*)(OS_HEAP_ADDRESS + OS_HEAP_SIZE_BYTES);
    int result = create_heap(&kernel_heap, (void*)OS_HEAP_ADDRESS, end_add, &kernel_heap_table);
    if(result<0){
        serial_write_string("Error creating kernel heap\n"); //implement the kernel panic later
    }
}

void* kmalloc(size_t size){
    return heap_malloc(&kernel_heap, size);
}

void kfree(void* ptr){
    heap_free(&kernel_heap, ptr);
}