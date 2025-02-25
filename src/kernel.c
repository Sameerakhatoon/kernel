#include "kernel.h"
#include "idt/idt.h"
#include "io/io.h"

extern void problem();
extern void out_byte(unsigned short port, unsigned char data);

void main_kernel() {
    serial_init();  // Initialize serial output
    // Send ANSI escape sequence to clear the terminal
    serial_write_string("\033[2J");  // Clear screen
    serial_write_string("\033[H");   // Move cursor to top-left
    serial_write_string("Hello, Serial!\n");
    init_idt();
    // problem();
    enable_interrupts();

    // out_byte(0x60, 0xff);
}

