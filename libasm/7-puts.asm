BITS 64

section .text
global asm_puts
extern my_strlen       ; Import my_strlen from 0-strlen.asm

; Prototype: size_t asm_puts(const char *str);
asm_puts:
    ; Input: RDI - pointer to the string (const char *str)

    call my_strlen            ; Call the length function (my_strlen)
    mov rbx, rax              ; Store the length in RBX for later use

    ; Set up for syscall to write
    mov rax, 1                ; Syscall number for sys_write (1)
    mov rdi, 1                ; File descriptor 1 is stdout
    mov rsi, RDI              ; Pointer to the string (input from RDI)
    mov rdx, rbx              ; Length of the string (in RBX)

    ; Perform the syscall
    syscall                    ; Invoke the syscall (write)

    ; Return the number of bytes written
    mov rax, rbx              ; Return value is the length of the string
    ret                        ; Return to the caller
