#include "idt.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../kernel.h"

void init_idt();

void set_idt(int interrupt_num, void* handler_add);

void idt_zero();

extern void load_idt(IdtRegisterDescriptor* ptr);

IdtDescriptor idt_descriptors[OS_TOTAL_INTERRUPTS];
IdtRegisterDescriptor idt_register_descriptor;

void init_idt(){
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idt_register_descriptor.limit = sizeof(idt_descriptors) - 1;
    idt_register_descriptor.base = (uint32_t)idt_descriptors;

    load_idt(&idt_register_descriptor);
    set_idt(0, idt_zero);

}

void set_idt(int interrupt_num, void* handler_add){
    IdtDescriptor* descriptor = &idt_descriptors[interrupt_num];
    descriptor->offset_1 = (uint32_t) handler_add & 0x0000FFFF;
    descriptor->offset_2 = (uint32_t) handler_add >> 16;
    descriptor->selector = KERNEL_CODE_SELECTOR;
    descriptor->type_attr = 0xEE; // 1110 1110, P(1) DPL(11) S(0) TYPE(1110)
    descriptor->zero = 0x00;
}

void idt_zero(){
    //divide by zero error, interrupt zerp
    serial_write_string("Divide by zero error\n");
    while(1); // halt the system, because after divide by zero instruction, above message will be printed, then the pc jumps back to same instruction, so it will be an infinite loop, so halt the system
}

// extern void load_idt(IdtRegisterDescriptor* ptr);