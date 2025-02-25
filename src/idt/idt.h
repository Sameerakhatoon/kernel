#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct IdtDescriptor IdtDescriptor;
struct IdtDescriptor {
    uint16_t offset_1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t zero; // unused, set to 0
    uint8_t type_attr; // type and attributes, see below
    uint16_t offset_2; // offset bits 16..31
} __attribute__((packed));

typedef struct IdtRegisterDescriptor IdtRegisterDescriptor;
struct IdtRegisterDescriptor {
    uint16_t limit; // size of the IDT - 1
    uint32_t base; // address of the first IdtDescriptor struct
} __attribute__((packed));

void init_idt();

#endif