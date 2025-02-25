section .asm

global load_idt
extern int21h_handler
global int21h
global enable_interrupts
global disable_interrupts
global no_interrupts
extern no_interrupts_handler
enable_interrupts:
    sti
    ret

disable_interrupts:
    cli
    ret


load_idt:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8] ; i.e., argument passsed to the function, if ebp then it points to ebp, if ebp+4, it points to the return address of the caller function!
    lidt [ebx]

    pop ebp
    ret

int21h: ; to handle keyboard interrupts
    cli ; clear interrupts
    pushad ; push all registers to stack
    call int21h_handler
    popad ; pop all registers from stack
    sti ; set interrupts
    iret

no_interrupts:
    cli ; clear interrupts
    pushad ; push all registers to stack
    call no_interrupts_handler
    popad ; pop all registers from stack
    sti ; set interrupts
    iret