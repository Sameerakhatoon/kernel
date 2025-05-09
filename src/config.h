#ifndef CONFIG_H
#define CONFIG_H

#define OS_TOTAL_INTERRUPTS 512
#define KERNEL_CODE_SELECTOR 0x08
#define KERNEL_DATA_SELECTOR 0x10

#define OS_HEAP_SIZE_BYTES 104857600

#define OS_HEAP_BLOCK_SIZE 4096

#define OS_HEAP_ADDRESS 0x01000000

#define OS_HEAP_TABLE_ADDRESS 0x00007E00

#define OS_SECTOR_SIZE 512

#define OS_MAX_FILESYSTEMS 12

#define OS_MAX_FILEDESCRIPTORS 512

#endif