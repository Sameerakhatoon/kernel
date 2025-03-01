#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kernel_heap.h"
#include "paging/paging.h"
#include "disk/disk.h"


extern void problem();
extern void out_byte(unsigned short port, unsigned char data);

static Paging4GBChunk* kernel_chunk = 0;

void main_kernel() {
    serial_init();  // Initialize serial output
    // Send ANSI escape sequence to clear the terminal
    serial_write_string("\033[2J");  // Clear screen
    serial_write_string("\033[H");   // Move cursor to top-left
    serial_write_string("Hello, Serial!\n");

    // Initialize the kernel heap
    init_kernel_heap();

    init_idt();
    // problem();
    // enable_interrupts();

    // out_byte(0x60, 0xff);

    // void* ptr = kmalloc(100);
    // void* ptr2 = kmalloc(5000);
    // if(ptr || ptr2){
    //     serial_write_string("Heap allocated\n");
    // }
    // else{
    //     serial_write_string("Heap allocation failed\n");
    // }

    // kfree(ptr);
    // kfree(ptr2);
    // void* ptr3 = kmalloc(100);
    // if(ptr3){
    //     serial_write_string("Heap allocated\n");
    // }
    // else{
    //     serial_write_string("Heap allocation failed\n");
    // }

    // Initialize paging
    kernel_chunk = paging_new_4gb(PAGING_READ_WRITE | PAGING_PRESENT | PAGING_USER_MODE);
    // Switch to the new page directory
    paging_switch(paging_chunk_get_page_directory(kernel_chunk));

    // char* ptr = kzalloc(4096);
    // paging_set(paging_chunk_get_page_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_PRESENT | PAGING_READ_WRITE | PAGING_USER_MODE);
    
    // Enable paging
    enable_paging();

    // char* ptr2 = (char*) 0x1000;
    // ptr2[0] = 'A';
    // ptr2[1] = 'B';
    // serial_write_string(ptr2);
    // serial_write_string(ptr);

    char buffer[512];
    disk_read_sector(0, 1, buffer);
    serial_write_string(buffer);


    enable_interrupts();
}



