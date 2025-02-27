#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"
#include "memory/heap/kernel_heap.h"

extern void problem();
extern void out_byte(unsigned short port, unsigned char data);

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

    void* ptr = kmalloc(100);
    void* ptr2 = kmalloc(5000);
    if(ptr || ptr2){
        serial_write_string("Heap allocated\n");
    }
    else{
        serial_write_string("Heap allocation failed\n");
    }

    kfree(ptr);
    kfree(ptr2);
    void* ptr3 = kmalloc(100);
    if(ptr3){
        serial_write_string("Heap allocated\n");
    }
    else{
        serial_write_string("Heap allocation failed\n");
    }
}



