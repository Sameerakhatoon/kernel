#include "memory.h"

void* memset(void* ptr, int c, size_t size){
    char* ptr_to_char = (char*) ptr;
    for(size_t i = 0; i < size; i++){
        ptr_to_char[i] = (char)c;
    }
    return ptr;
}