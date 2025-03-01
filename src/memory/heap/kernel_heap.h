#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include "stdint.h"
#include "stddef.h"

void init_kernel_heap();

void* kmalloc(size_t size);

void kfree(void* ptr);

void* kzalloc(size_t size);

#endif