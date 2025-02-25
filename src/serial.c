#include "kernel.h"

#define COM1 0x3F8  // COM1 base address

void outb(unsigned short port, unsigned char val) {
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

unsigned char inb(unsigned short port) {
    unsigned char ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Initialize serial port
void serial_init() {
    outb(COM1 + 1, 0x00);  // Disable all interrupts
    outb(COM1 + 3, 0x80);  // Enable DLAB (set baud rate divisor)
    outb(COM1 + 0, 0x03);  // Set divisor to 3 (lo byte) 38400 baud
    outb(COM1 + 1, 0x00);  // (hi byte)
    outb(COM1 + 3, 0x03);  // 8 bits, no parity, one stop bit
    outb(COM1 + 2, 0xC7);  // Enable FIFO, clear them, 14-byte threshold
    outb(COM1 + 4, 0x0B);  // IRQs enabled, RTS/DSR set
}

// Write a character to serial
void serial_write(char c) {
    while (!(inb(COM1 + 5) & 0x20));  // Wait for the buffer to be empty
    outb(COM1, c);
}

// Write a string to serial
void serial_write_string(const char *str) {
    // serial_write('\n');
    while (*str) {
        serial_write(*str++);
    }
}
