section .asm

global in_byte
global in_word
global out_byte
global out_word

in_byte: ; IN AL,DX	Input byte from I/O port in DX into AL.
    push ebp ; Save the base pointer
    mov ebp, esp ; Set the base pointer

    xor eax, eax ; Clear the eax register

    mov edx, [ebp + 8] ; Move the argument into edx, as ebp+4 is return address, ebp is caller's bp address
    in al, dx ; Input byte from I/O port in DX into AL.

    pop ebp ; Restore the base pointer
    ret

in_word: ; 	IN AX,DX	Input word from I/O port in DX into AX.
    push ebp ; Save the base pointer
    mov ebp, esp ; Set the base pointer

    xor eax, eax ; Clear the eax register

    mov edx, [ebp + 8] ; Move the argument into edx, as ebp+4 is return address, ebp is caller's bp address
    in ax, dx ; Input word from I/O port in DX into AX.

    pop ebp ; Restore the base pointer
    ret

out_byte:
    push ebp ; Save the base pointer
    mov ebp, esp ; Set the base pointer

    mov eax, [ebp + 12] ; Move 2nd argument into eax
    mov edx, [ebp + 8] ; Move 1st argument into edx

    out dx, al ; Output byte in AL to I/O port in DX.

    pop ebp ; Restore the base pointer
    ret

out_word:
    push ebp ; Save the base pointer
    mov ebp, esp ; Set the base pointer

    mov eax, [ebp + 12] ; Move 2nd argument into eax
    mov edx, [ebp + 8] ; Move 1st argument into edx

    out dx, ax ; Output byte in AX to I/O port in DX.

    pop ebp ; Restore the base pointer
    ret
