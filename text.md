- bios(Basic Input/Output System) is stored in rom
- memory is accessed in a linear fashion
- memory is divided into pages
- page table is used to map virtual address to physical address
- tlb is used to cache the page table
- various memory schemes used by processor, like segmentation, paging, etc
- memory management unit is used to translate virtual address to physical address
- memory protection is done by setting the access rights in the page table

Boot Process
- BIOS is stored in ROM
- BIOS loads the boot loader from the boot sector of the disk in address 0x7c00 in case of x86
- Boot loader loads the kernel into memory
- Kernel initializes the hardware and starts the init process
- Init process starts the user space processes

BIOS
- Basic Input/Output System
- Stored in ROM
- Provides basic input/output services
- BIOS is the first program that runs when the computer is powered on
- How does the processor know where to start executing code?
  - Processor is hardwired to start executing code at a fixed address
  - This address is the reset vector
  - The reset vector points to the BIOS code
- BIOS is responsible for loading the boot loader from the boot sector of the disk
- Has a set of functions that can be called by the boot loader
  - example of functions are read_disk, write_disk, etc
- Initializes essential hardware, like memory, keyboard, etc
- Has drivers for essential hardware, like disk, keyboard, etc
- BIOS looks for boot loader to boot by searching all storage mediums for boot signature 0x55AA at the end of the boot sector
  - 511th byte & 512th byte has boot signature, if it was present, then BIOS will load the boot loader
- BIOS in itself is a kernel, 
  - BIOS contains routines to assist boot loader in booting kernel
  - BIOS rouutines are generic and a standard
  - BIOS contains routines to initialize hardware, like memory, keyboard, etc
  - BIOS contains routines to load the boot loader from the boot sector of the disk
  - BIOS contains routines to load the kernel into memory
  - BIOS contains routines to pass control to the kernel
- BIOS is 16 bit code, so as to run on processor in real mode or compatibility mode once the processor is powered on

Boot Loader
- Boot loader is the first program that runs when the computer is powered on
- Responsible for loading kernel of os
- generally small
- resides in the boot sector of the disk
- loads the kernel into memory
- passes control to the kernel
- Uses BIOS functions assist it's operation

Booting
- Power on self test(POST) is run
- BIOS is loaded
- BIOS loads the boot loader from the boot sector of the disk
- Boot loader loads the kernel into memory
- Kernel initializes the hardware and starts the init process
- Init process starts the user space processes
- On booting, processor is in compatibility mode, aka real mode, aka 16 bit mode
- Processor is switched to protected mode, aka 32 bit mode, by the kernel
- Boot loader uses functions in BIOS to load the kernel

Interrupt list
https://www.ctyme.com/intr/int.htm

Real Mode:
- 20-bit address lines - 2^20 = 1048576 bytes = 1 MB
- registers in the x86 architecture are only 16-bit wide
- 1 MB OF RAM ACCESSIBLE
  - explanation: memory access is through segment:offset, where, 
- BASED ON THE ORIGINAL X86 DESIGN
- NO SECURITY(memory, hardware, os)
- 8 or 16 BIT'S ACCESSIBLE AT ONE TIME
  - 2^16 = 64 KB, but 1 MB of memory is accessible, 
  - segmentation:
    - MEMORY IS ACCESSED BY A SEGMENT AND AN OFFSET
    - PROGRAMS CAN BE LOADED IN DIFFERENT AREAS OF MEMORY BUT RUN WITHOUT PROBLEMS
    - MULTIPLE SEGMENTS ARE AVAILABLE THROUGH THE USE OF SEGMENT REGISTERS
    • CS-Code Segment
    • SS - Stack Segment
    • DS - Data Segment
    • ES - Extra Segment
    • Take the segment register multiply it by 16 and add the offset, to get the absolute address
    - how 1 mb can be accessed? (16*(2^16))+(2^16) = 1114112 bytes = 1 MB
    - Different Instructions Use Different Segment Registers
      • "lodsb" uses the DS:SI register combination, loads the AL reg with the byte at DS:SI and increments SI
      • "lodsw" uses the DS:SI register combination, loads the AX reg with the word at DS:SI and increments SI
      • "stosb" uses the ES:DI register combination, stores the AL reg at ES:DI and increments DI
      • "stosw" uses the ES:DI register combination, stores the AX reg at ES:DI and increments DI
      • "movsb" uses the DS:SI and ES:DI register combination, moves the byte at DS:SI to ES:DI and increments both SI and DI
      • "movsw" uses the DS:SI and ES:DI register combination, moves the word at DS:SI to ES:DI and increments both SI and DI
Code Mnemonic Description
AC LODS m8 Load byte at address DS: (E)SI into AL
AD LODS m16 Load word at address DS: (E)SI into AX
AD LODS m32 Load doubleword at address DS:(E)SI into EAX
AC LODSB Load byte at address DS: (E)SI into AL
AD LODSW Load word at address DS: (E)SI into AX
AD LODSD Load doubleword at address DS: (E)SI into EAX

PROGRAMS CAN BE LOADED IN DIFFERENT AREAS OF MEMORY BUT RUN WITHOUT PROBLEMS
Imagine we have two programs loaded into memory, both were assembled with the origin being zero
Program 1 uses segment 0x7C0 for all its segment registers
Program 1 is loaded at address 0x7C00
Program 2 uses segment 0x7D0 for all its segment registers
Program 2 is located at address 0x7D00
We swap our segment registers when switching to the other process
We restore all the registers of the process we are switching too
It resumes as if we never switched process at all

Address = Segment*16 + SegmentPointer

Interrupt Vector Table (IVT) is located at memory address 0x0000 to 0x03FF, first 1 KB (1024 bytes) of RAM.
- 256 interrupt vectors (INT 0x00 to INT 0xFF)
- Each vector is 4 bytes long
- so, first 1024 bytes of RAM is reserved for IVT, should not be used by any other program

https://wiki.osdev.org/FAT#:~:text=Boot%20Record.-,BPB%20(BIOS%20Parameter%20Block),-The%20boot%20record

The CPU starts executing the boot sector at 0x7C00 in memory.
The boot sector is 512 bytes long.
bios loads the boot sector into memory at 0x7C00

WHAT ARE INTERRUPTS?
Interrupts are like subroutines, but you don't need to know the memory address to invoke them
Interrupts are called through the use of interrupt numbers rather than memory addresses
Interrupts can be setup by the programmer. For example you could set up interrupt "0x32" and have it point to your code. Then when someone does "int 0x32" it will invoke your code

WHAT HAPPENS WHEN YOU INVOKE AN INTERRUPT?
Processor is interrupted
Old state saved on the stack
Interrupt executed

TABLE DESCRIBING 256 INTERRUPT HANDLERS
ENTIRES CONTAIN 4 BYTES (OFFSET:SEGMENT)
INTERRUPTS ARE IN NUMERICAL ORDER IN THE TABLE

Exceptions https://wiki.osdev.org/Exceptions

DISK ACCESS AND HOW IT WORKS
FILES DO NOT EXIST
1. Filesystems are kernel implemented they are not the responsibility of the hard disk
2. Implementing a filesystem requires the kernel programmer to create a filesystem driver for the target filesystem
3. Special data structures, as meta data for the actual data, that kernel uses to keep track of the files
4. The kernel uses the filesystem driver to read and write to the disk in the filesystem's format
5. Disk holds data blocks called sectors

DATA IS READ AND WRITTEN IN SECTORS
- Data is read and written in sectors typically 512 byte blocks
- Reading the sector of a disk will return 512 bytes of data for the chosen sector

CHS(CYLINDER HEAD SECTOR)
- Sectors are read and written by specifying a "head", "track" and "sector"

LBA(LOGICAL BLOCK ADDRESS)
- This is the modern way of reading from a hard disk, rather than specify "head", "track" and "sector" we just specify a number that starts from zero and goes up
- LBA allows us to read from the disk as if we are reading blocks from a very large file
- LBA, O = first sector on the disk
- LBA 1 second sector on the disk

CALCULATING LBA
Let's say we want to read the byte at position 58376 on the disk how do we do it?
LBA = 58376/512 = 114
offset = 58376%512 = 8

BIOS DISK ROUTINES
In 16 bit real mode the bios provides interrupt 13h for disk operations
In 32 bit mode you have to create your own disk driver which is a little more complicated

% if 0
DISK - READ SECTOR(S) INTO MEMORY
AH = 02h, 2nd function of int 13h, read sectors into memory
AL = number of sectors to read (must be nonzero)
CH = low eight bits of cylinder number
CL = sector number 1-63 (bits 0-5)
high two bits of cylinder (bits 6-7, hard disk only)
DH = head number
DL = drive number (bit 7 set for hard disk)
ES:BX -> data buffer

Return:
CF set on error
if AH = 11h (corrected ECC error), AL = burst length
CF clear if successful
AH = status (see #00234)
AL = number of sectors transferred (only valid if CF set for some
BIOSes)
%endif

Protected Mode
Protected mode is a processor state in x86 architectures which gives access to memory protection, 4GB address space and much more.
CAN PROVIDE MEMORY AND HARDWARE PROTECTION
DIFFERENT MEMORY SCHEMES
4 GB OF MEMORY ADDRESSABLE

Memory And Hardware Protection
• Protected Mode Allows You To Protect Memory From Being Accessed
• Protected Mode Can Prevent User Programs Talking With Hardware
Ring 3 Applications - least privileged, prevents overriding kernel memory, accessing hardware, other program address space, using priviliged instructions(sti, cli, in, out), etc, can't talk to hardware, can't access all memory, can't execute all instructions, can't change the processor state, can't change the interrupt vector table, can't change the page table
Ring 2 Device drivers
Ring 1 Device drivers
Ring 0 Kernel - most privileged, can talk to hardware, can access all memory, can execute all instructions, can change the processor state, can change the interrupt vector table, 

user programs talk to kernel through inetrrupts, putting processor in ring 0, kernel mode, can talk to hardware, can access all memory, can execute all instructions, can change the processor state, can change the interrupt vector table, does the job, executes the interrupt, back to ring 3, user mode

Different Memory Schemes
1. Selectors (CS, DS, ES, SS) ect...
2. Paging (Remapping Memory Addresses)

Selector Memory Scheme
• Our segmentation registers become selector registers
• Selectors point to data structures that describe memory ranges and the permissions(ring level) required to access a given range

Paging Memory Scheme
• Memory is virtual and what you address can point to somewhere entirely different in memory Page Directory
• Memory protection is easier to control, a program can't access memory outside of its page
• Paging is the most popular choice for memory schemes with kernels/operating systems
- directory of pointers to page tables, page table entries, page table entries point to physical memory
- the physical addresses which the virtual addresses point to are multiple of 4096, 4 KB

4GB Of Addressable Memory
We gain access to 32-bit instructions and can easily work with 32-bit registers instead of 16-bit registers and instructions size of 16 bits in real mode
We can address up to 4GB of memory at any time and we are no longer limited to the 1MB of memory provided by real mode

https://wiki.osdev.org/Protected_Mode

https://wiki.osdev.org/Global_Descriptor_Table

https://chatgpt.com/share/67bb1ea3-2804-8002-abb4-f6369e8de675

https://chatgpt.com/share/67bb1eb2-ec00-8002-acb6-6bec2ea36afe

Entering Protected Mode
Before switching to protected mode, you must:

Disable interrupts, including NMI (as suggested by Intel Developers Manual).
Enable the A20 Line.
Load the Global Descriptor Table with segment descriptors suitable for code, data, and stack.
Whether the CPU is in Real Mode or in Protected Mode is defined by the lowest bit of the CR0 or MSW register.

This example loads a descriptor table into the processor's GDTR register, and then sets the lowest bit of CR0:

cli            ; disable interrupts
lgdt [gdtr]    ; load GDT register with start address of Global Descriptor Table
mov eax, cr0 
or al, 1       ; set PE (Protection Enable) bit in CR0 (Control Register 0)
mov cr0, eax

; Perform far jump to selector 08h (offset into GDT, pointing at a 32bit PM code segment descriptor) 
; to load CS with proper PM32 descriptor)
jmp 08h:PModeMain

PModeMain:
; load DS, ES, FS, GS, SS, ESP

Step-by-Step Debugging (Instruction by Instruction)
1️⃣ Set Up QEMU & GDB
In Terminal 1, run QEMU with debugging enabled:

sh
Copy code
qemu-system-i386 -drive format=raw,file=build/boot.bin -S -gdb tcp::1234 -nographic
This pauses QEMU and waits for GDB to connect.

In Terminal 2, open GDB and connect:

sh
Copy code
gdb
(gdb) set architecture i8086
(gdb) target remote localhost:1234
2️⃣ Step Through Instructions
Now, use these commands to debug instruction by instruction:

Step One Instruction
gdb
Copy code
(gdb) stepi
Executes one instruction at a time.
Continue Execution Until a Breakpoint
gdb
Copy code
(gdb) continue
Runs until a breakpoint or infinite loop.
Run Until a Specific Instruction
gdb
Copy code
(gdb) until 0x7c7f
Runs the code until reaching address 0x7c7f.
View Current Instruction
gdb
Copy code
(gdb) x/i $pc
Shows the instruction at the current program counter (PC).
View Multiple Instructions Around the PC
gdb
Copy code
(gdb) x/10i $pc
Displays 10 instructions starting from the current PC.
3️⃣ Using Breakpoints
Breakpoints allow you to pause execution at specific locations.

Set a Breakpoint at 0x7c00
gdb
Copy code
(gdb) break *0x7c00
Set a Breakpoint at a Specific Instruction
gdb
Copy code
(gdb) break *0x7c7f
List All Breakpoints
gdb
Copy code
(gdb) info breakpoints
Remove a Breakpoint
gdb
Copy code
(gdb) delete 1
(Remove breakpoint #1, replace 1 with the breakpoint number.)

4️⃣ View Registers & Memory
Check Registers
gdb
Copy code
(gdb) info registers
Dump All Registers Continuously
gdb
Copy code
(gdb) display /x $eax $ebx $ecx $edx $esp $ebp $esi $edi $eip
(This will auto-print registers after every step.)

Inspect Memory at 0x7c00
gdb
Copy code
(gdb) x/16xb 0x7c00
(Shows 16 bytes in hex starting at 0x7c00.)

View Stack
gdb
Copy code
(gdb) x/16xw $esp
(Shows 16 words from the stack.)

5️⃣ Enable Assembly View in GDB (Optional)
If you want GDB to show assembly instructions in real-time, use:

gdb
Copy code
(gdb) layout asm
Use Ctrl+L to refresh if the screen gets stuck.

6️⃣ Exit Debugging
To exit GDB:

gdb
Copy code
(gdb) quit
To stop QEMU:

sh
Copy code
Ctrl+C   # (In Terminal 1)


disassemble 0x7c00, 0x7e00 #since 1 sector of boot loader is 512 bytes, 0x7c00 to 0x7e00 is the boot loader, bootloader can still have labels after 512 bytes, then load into memory, and then jump to the label

--------------

- after switching to protected mode, can't read from the disk anymore using int 13, 2 function, have to write the disk driver

https://wiki.osdev.org/ATA_PIO_Mode

https://chatgpt.com/share/67bb7e1c-4abc-8002-a05f-8cabfd1cf882

linkder.ld
OUTPUT_FORMAT(binary)     ; Specify that the output format should be raw binary

SECTIONS {
    . = 1M;              ; Start loading sections at 1MB memory location

    .text : {            ; Code section
        *(.text)         ; Include all .text sections from object files
    }
    .rodata : {          ; Read-only data section
        *(.rodata)       ; Include all .rodata sections
    }
    .data : {            ; Initialized data section
        *(.data)         ; Include all .data sections
    }
    .bss : {             ; Uninitialized data section
        *(COMMON)        ; Include COMMON symbols
        *(.bss)         ; Include all .bss sections
    }
}

What is text mode?
TEXT MODE ALLOWS YOU TO WRITE ASCII TO VIDEO MEMORY
TEXT MODE SUPPORTS 16 UNIQUE COLOURS
NO NEED TO SET INDIVIDUAL SCREEN PIXELS FOR PRINTING CHARACTERS

Text mode allows you to write ascii to video memory
• You write ASCII characters into memory starting at address 0xB8000 for coloured displays
• Or for monochrome displays address OxB0000
• Each ascii character written to this memory has its pixel equivalent outputted to the monitor.

Each character takes up two bytes
• Byte 0 = ascii character e.g 'A'
• Byte 1 = Colour code


INTERRUPT DESCRIPTOR TABLE 
- DESCRIBES HOW INTERRUPTS ARE INVOKED IN PROTECTED MODE
- CAN BE MAPPED ANYWHERE IN MEMORY
- DIFFERENT FROM THE INTERRUPT VECTOR TABLE
- Array of Interrupt descriptors

https://chatgpt.com/share/67bc8599-d0e8-8002-bf29-3bc828cd3e40

Interrupt Descriptor Structure
struct idt_desc
{
uint16_t offset_1; // offset bits 0..15
uint16_t selector; // a code segment selector in GDT or LDT
uint8_t zero;
// unused, set to 0
uint8_t type_attr; // type and attributes, see below
uint16_t offset_23; // offset bits 16..31
} _attribute__((packed));

struct idtr_desc
{
uint16_t limit;
uint32_t base;
} _attribute__((packed));

Loading interrupt descriptor table
idt_load:
push ebp
mov ebp, esp
mov ebx, [ebp+8]
lidt [ebx] ; pointer to idtr
pop ebp
ret


Final Notes
• Interrupt Descriptor Table can be defined where we like in memory
• Interrupt Descriptor Tables are Setup differently than the interrupt vector table.
• During an interrupt certain properties can be pushed to the stack. The rules involved with this are quite complicated so we will discuss them as they come and they do not always apply.

https://chatgpt.com/share/67bc8599-d0e8-8002-bf29-3bc828cd3e40

https://wiki.osdev.org/Interrupt_Descriptor_Table

https://c9x.me/x86/html/file_module_x86_id_139.html

ESP (Stack Pointer) and EBP (Base Pointer) are key registers used to manage function calls and stack frames

ESP stands for Extended Stack Pointer.
It points to the top of the stack, where the most recent data is stored.
It changes dynamically when data is pushed or popped.

EBP stands for Extended Base Pointer.
It remains constant during a function call, unlike ESP, which moves.
It helps access function parameters and local variables.

How EBP is Used
The function saves the old base pointer (push ebp).
It sets up a new base pointer (mov ebp, esp).
Function arguments and local variables are accessed using EBP.

When a function is called:

The return address is pushed onto the stack (by call instruction).
The previous EBP is saved (push ebp).
A new stack frame is created (mov ebp, esp).

int add(int a, int b) {
    return a + b;
}

add:
    push ebp        ; Save old EBP
    mov ebp, esp    ; Set new stack frame
    mov eax, [ebp+8] ; Load 'a' into EAX
    add eax, [ebp+12] ; Add 'b' to EAX
    pop ebp         ; Restore previous stack frame
    ret             ; Return (uses address in [EBP+4])

PROGRAMMABLE INTERRUPT CONTROLLER
ALLOWS HARDWARE TO INTERRUPT THE PROCESSOR STATE
PROGRAMMABLE
REQUIRES INTERRUPT ACKNOWLEDGMENT

Allows Hardware To Interrupt The Processor
• The programmable interrupt controller allows different types of hardware to interrupt the processor such as the hard disk, keyboard and more..

Understanding IRQ's
• IRQ's are mapped to a starting interrupt for example lets choose 0x20.
• IRQ 0 would then be interrupt 0x20
• IRQ 1 would then be interrupt 0x21
• IRQ 2 would then be interrupt 0x22

By default some of the IRQ's are mapped to interrupts 8-15 this is a problem as these interrupts are reserved in protected mode for exceptions so we are required to remap the PIC(Programmable Interrupt Controller)

Master Vs Slave
• The system has two PIC (Programmable Interrupt Controller) one for master ports and the other for slave ports
• The master handles IRQ 0-7
• The slave handles IRQ 8-15

Master Vs Slave
• The system has two PIC (Programmable Interrupt Controller) one for master ports and the other for slave ports
• The master handles IRQ 0-7
• The slave handles IRQ 8-15

Remapping the Master PIC
setup_pic:
  ; Initialize some flags in the PIC's
  mov al, 00010001b; b * 4 = 1 Init; b * 3 = 8 Edge; b * 1 = 0 : Cascade; b*theta = 1 : Need 4th init step
  out 0x20, al; Tell master
  mov al, 0x20; Master IRQO should be on INT 0x20 (Just after intel exceptions)
  out 0x21, al
  mov al, 00000001b; b * 4 = 0 FNM; 63-2-00: Master/Slave set by hardware; b * 1 = 0 Not AEOΙ; b*theta = 1 x86 mode
  out 0x21, al
ret

Interrupt Acknowledgment
• You must let the PIC controller know when you have handled the interrupt

// Let the PIC know we ackowledge the ISR
outb (PIC1, PIC_EOI); //PIC1 is the base address of the master PIC, PIC_EOI is the end of interrupt command code

// ISR Definitions
#define PIC1 0x20 /* 10 base address for master PIC */
#define PIC2 0XA0 /* 10 base address for slave PIC */
#define PIC1_COMMAND PIC1
#define PICI_DATA (PIC1+1)
#define PIC2_COMMAND PIC2
#define PIC2_DATA (PIC2+1)
#define PIC_ΕΟΣ 0x20 / End-of-interrupt command code */

