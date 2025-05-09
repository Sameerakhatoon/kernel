[BITS 32]          ; This tells the assembler that we're writing 32-bit code.

global _start      ; Declare _start as a global symbol so the linker can find it.
extern main_kernel ; Declare start_kernel as an external symbol.
global problem
; Define segment selectors (used to access memory in protected mode)
CODE_SEG equ 0x08  ; Code segment selector (standard in 32-bit protected mode)
DATA_SEG equ 0x10  ; Data segment selector (for accessing data in memory)

_start:
    ; --- Setting up protected mode data segments ---
    
    mov ax, DATA_SEG  ; Load the data segment selector (0x10) into AX register.
    mov ds, ax        ; Set DS (Data Segment) to point to the data segment.
    mov es, ax        ; Set ES (Extra Segment) to point to the data segment.
    mov fs, ax        ; Set FS (General-purpose segment) to the data segment.
    mov gs, ax        ; Set GS (General-purpose segment) to the data segment.
    mov ss, ax        ; Set SS (Stack Segment) to the data segment.
    
    ; --- Setting up the stack ---
    
    mov ebp, 0x00200000  ; Set EBP (Base Pointer) to memory address 0x00200000.
    mov esp, ebp         ; Copy EBP to ESP (Stack Pointer) to initialize the stack.
    
    ; --- Enabling the A20 line ---
    ; The A20 line must be enabled to access memory above 1MB.
    ; The A20 line was originally disabled for compatibility with very old computers.

    in al, 0x92   ; Read the value from I/O port 0x92 (System Control Port A).
    or al, 2      ; Set bit 1 (A20 enable bit) in the AL register.
    out 0x92, al  ; Write the modified value back to port 0x92, enabling the A20 line.

    ; --- Remap the PIC (Programmable Interrupt Controller) ---
    ; The PIC is used to manage hardware interrupts. We need to remap it to avoid conflicts, as the default addresses overlap with CPU exceptions.
    ; Initialize some flags in the PIC's
    mov al, 00010001b; 
    out 0x20, al; Tell master
    mov al, 0x20; Master IRQO should be on INT 0x20 (Just after intel exceptions)
    out 0x21, al
    mov al, 00000001b; 
    out 0x21, al

    ; --- Entering an infinite loop ---
    ; This keeps the processor in an idle state, preventing it from executing garbage instructions.
    call main_kernel  ; Call the start_kernel function from the C code.
    jmp $         ; Infinite loop (jmp to itself, so the CPU never moves forward).

problem: ; void function to produce a problem, divide by zero
    push ebp
    mov ebp, esp
    mov eax, 1    ; Change 0 to 1 to prevent divide-by-zero
    div eax
    pop ebp
    iret
    


times 512 - ($ - $$) db 0  ; Fill the rest of the sector with zeros to make it 512 bytes long.