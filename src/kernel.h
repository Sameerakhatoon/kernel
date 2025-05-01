#ifndef KERNEL_H
#define KERNEL_H

void main_kernel();
void serial_init();
void serial_write_string(const char *str);

#define OS_MAX_PATH_LENGTH 108

#define ERROR(value) (void*)(value)
#define ERROR_I(value) ((int)(value))
#define ISERR(value) ((int)(value) < 0)


#endif