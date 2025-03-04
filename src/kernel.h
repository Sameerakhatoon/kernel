#ifndef KERNEL_H
#define KERNEL_H

void main_kernel();
void serial_init();
void serial_write_string(const char *str);

#define OS_MAX_PATH_LENGTH 108

#endif