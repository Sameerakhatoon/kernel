section .asm

global load_idt

load_idt:
    push ebp
    mov ebp, esp

    mov ebx, [ebp+8] ; i.e., argument passsed to the function, if ebp then it points to ebp, if ebp+4, it points to the return address of the caller function!
    lidt [ebx]

    pop ebp
    ret
