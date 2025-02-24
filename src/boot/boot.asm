ORG 0x7c00          ; Set the origin to 0x7C00, where the BIOS loads the bootloader
BITS 16            ; Assemble the code for 16-bit mode

; Define segment offsets based on the Global Descriptor Table (GDT)
CODE_SEG equ gdt_code - gdt_start ; Code segment offset
DATA_SEG equ gdt_data - gdt_start ; Data segment offset

_start:
    jmp short start ; Jump to the "start" label to ensure execution begins correctly
    nop            ; No operation, just padding

times 33 db 0     ; Reserve 33 bytes for the BIOS Parameter Block (BPB), which contains disk information

start:
    jmp 0x0000:next ; Set CS (Code Segment) to 0x0000 and jump to "next"

next:
    cli             ; Disable interrupts to prevent unexpected behavior during setup
    mov ax, 0x00    ; Set AX register to 0 (used to clear segment registers)
    mov ds, ax      ; Set Data Segment (DS) to 0
    mov es, ax      ; Set Extra Segment (ES) to 0
    mov ss, ax      ; Set Stack Segment (SS) to 0
    mov sp, 0x7c00  ; Set Stack Pointer (SP) to 0x7C00 (bootloader memory location)
    sti             ; Enable interrupts again

load_protected:
    cli             ; Disable interrupts before switching to protected mode
    lgdt [gdt_descriptor] ; Load the Global Descriptor Table (GDT)
    mov eax, cr0    ; Load control register CR0 into EAX
    or eax, 0x1     ; Set the first bit of CR0 to enable protected mode
    mov cr0, eax    ; Write back to CR0, activating protected mode
    jmp CODE_SEG:load_32 ; Far jump to 32-bit mode using the new code segment, at 0x0100000 where the kernel be loaded

; ---- Global Descriptor Table (GDT) ----
gdt_start:
    ; Null Descriptor (Must be present as the first entry in GDT)
gdt_null:
    dd 0 ; First 32 bits
    dd 0 ; Second 32 bits (total 64-bit null descriptor)

; Code Segment Descriptor
gdt_code:
    dw 0xffff         ; Segment limit (first 16 bits)
    dw 0             ; Base address (first 16 bits)
    db 0             ; Base address (next 8 bits)
    db 0x9a          ; Access byte (code segment, executable, readable)
    db 11001111b     ; Flags: 4-bit flags + 4-bit segment limit
    db 0             ; Base address (last 8 bits)

; Data Segment Descriptor
gdt_data:
    dw 0xffff         ; Segment limit (first 16 bits)
    dw 0             ; Base address (first 16 bits)
    db 0             ; Base address (next 8 bits)
    db 0x92          ; Access byte (data segment, writable)
    db 11001111b     ; Flags: 4-bit flags + 4-bit segment limit
    db 0             ; Base address (last 8 bits)

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1 ; Size of the GDT (limit field)
    dd gdt_start               ; Address of the GDT

[BITS 32]  ; Switch to 32-bit mode instructions
load_32:
    ; Since we are now in protected mode, BIOS interrupts (e.g., INT 0x13 for disk access) will no longer work.
    ; We must use a custom disk driver to load the kernel from the disk.
    
    mov eax, 1       ; First sector to read
    mov ecx, 100     ; Number of sectors to read (adjust based on kernel size)
    mov edi, 0x0100000 ; Load kernel to 1MB memory address
    call ata_lba_read ; Call function to read from disk (ATA driver required)
    jmp CODE_SEG:0x0100000 ; Jump to the loaded kernel code

ata_lba_read:
    ; Function to read sectors from disk using the ATA protocol
    ; Input: EAX = LBA (Logical Block Address) of the first sector to read
    ;        ECX = Number of sectors to read
    ;        EDI = Destination memory address to load the sectors
    ; Output: Sectors are loaded into memory at the specified address
    mov ebx, eax     ; Copy LBA to EBX
    ; send highest 8 bits of LBA to port 0x1F6 or the controller
    shr ebx, 24     ; Shift right by 24 bits to get the highest 8 bits
    or ebx, 0xE0    ; Set the drive number (0xE0 = master, 0xF0 = slave)
    mov dx, 0x1F6   ; I/O port for the ATA controller
    out dx, al      ; Send the highest 8 bits of LBA to the controller
    ; send total number of sectors to read to port 0x1F2
    mov eax, ecx    ; Copy number of sectors to read to EAX
    mov dx, 0x1F2   ; I/O port for the ATA controller
    out dx, al      ; Send the number of sectors to read to the controller
    ; send LBA to port 0x1F3-0x1F5
    mov eax, ebx    ; Copy LBA to EAX
    mov dx, 0x1F3   ; I/O port for the ATA controller
    out dx, al      ; Send the lowest 8 bits of LBA to the controller
    ; send next 8 bits of LBA to port 0x1F4
    mov dx, 0x1F4   ; I/O port for the ATA controller
    mov eax, ebx    ; Copy LBA to EAX
    shr eax, 8      ; Shift right by 8 bits to get the next 8 bits
    out dx, al      ; Send the next 8 bits of LBA to the controller
    ; send next 8 bits of LBA to port 0x1F5
    mov dx, 0x1F5   ; I/O port for the ATA controller
    mov eax, ebx    ; Copy LBA to EAX
    shr eax, 16     ; Shift right by 16 bits to get the next 8 bits
    out dx, al      ; Send the next 8 bits of LBA to the controller
    ; send read command to port 0x1F7
    mov dx, 0x1F7   ; I/O port for the ATA controller
    mov al, 0x20    ; Command to read sectors (0x20 = read with retry)
    out dx, al      ; Send the read command to the controller
    
; read all sectors into memory
next_sector:
    push ecx ; so we can restore it later

; wait for the disk to be ready
wait_ready:
    mov dx, 0x1F7 ; I/O port for the ATA controller
    in al, dx     ; Read the status register
    test al, 8    ; Check if the drive is ready (bit 3 set)
    jz wait_ready ; Loop until the drive is ready

; read 256 words (512 bytes) from the data register at a time
read_sector:
    mov dx, 0x1F0 ; I/O port for the ATA data register
    mov ecx, 256   ; Number of words to read (512 bytes)
    rep insw      ; Repeat the input operation to read 256 words (512 bytes) from the data register

    pop ecx ; restore the original value of ECX
    loop next_sector ; Decrement ECX and loop until all sectors are read

    ; Once all sectors are read, return to the calling function
    ret
    
; Fill remaining space with zeros to make the boot sector 512 bytes

times 510-($-$$) db 0

; Boot signature (0xAA55) - Required for BIOS to recognize the bootloader
dw 0xaa55  ; Little-endian format (55AA on disk)
