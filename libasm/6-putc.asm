BITS 64

section .bss
    buffer resb 1            ; Reserve 1 byte for the character to print

section .text
global asm_putc

asm_putc:
    ; Save the character from rdi
    mov [buffer], dil        ; Store the character in the buffer

    ; Prepare for the syscall
    mov rax, 1               ; syscall number for sys_write
    mov rdi, 1               ; file descriptor: stdout
    lea rsi, [buffer]        ; pointer to the buffer
    mov rdx, 1               ; number of bytes to write

    syscall                   ; Invoke syscall to write the character

    mov rax, 1               ; Return the number of bytes written
    ret                       ; Return from function