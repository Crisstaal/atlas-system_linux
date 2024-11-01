BITS 64

section .text
global asm_puts

asm_puts:
    ; Input: RDI - pointer to the string (const char *str)
    ; The return value will be in RAX (size_t)

    call my_strlen          ; Call the length function (my_strlen)
    mov rbx, rax           ; Store the length in RBX for later use
    
    ; Prepare for the syscall
    mov rax, 1        ; syscall number for sys_write
    mov rdi, 1        ; file descriptor: stdout
    mov rsi, RDI      ; rsi will point to the string
    mov rdx, rBx      ; length of the string

    syscall            ; Invoke syscall to write the string

    mov rax, rbx           ; Return value is the length of the string
    ret                ; Return from function