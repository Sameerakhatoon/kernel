[BITS 32]
section.asm
global paging_load_directory
global enable_paging
paging_load_directory:
  push ebp
  mov ebp, esp
  mov eax, [ebp+8]
  mov cr3, eax ; pass the address of the page directory to cr3 register which was passed to the function as an argument
  pop ebp
  ret
enable_paging:
  push ebp
  mov ebp, esp
  mov eax, cr0
  or eax, 0x80000000 ; set the paging bit in the cr0 register, changing the processor to paging mode
  mov cr0, eax ; pass the modified cr0 register back to the cr0 register
  pop ebp
  ret