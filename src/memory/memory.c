#include "memory.h"

void* memset(void* ptr, int c, size_t size){
    char* ptr_to_char = (char*) ptr;
    for(size_t i = 0; i < size; i++){
        ptr_to_char[i] = (char)c;
    }
    return ptr;
}

int memcmp(const void* ptr1, const void* ptr2, size_t size){
    const char* ptr1_to_char = (const char*) ptr1;
    const char* ptr2_to_char = (const char*) ptr2;
    for(size_t i = 0; i < size; i++){
        if(ptr1_to_char[i] != ptr2_to_char[i]){
            return ptr1_to_char[i] - ptr2_to_char[i];
        }
    }
    return 0;
}