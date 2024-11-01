section .text
global asm_putc

asm_putc:
    ; Reserve a buffer directly in the text section
    ; Prepare a static buffer for writing
    sub rsp, 8              ; Create space on the stack
    mov byte [rsp], dil     ; Store the character in the stack

    ; Prepare for the syscall
    mov rax, 1              ; syscall number for sys_write
    mov rdi, 1              ; file descriptor: stdout
    lea rsi, [rsp]          ; pointer to the character
    mov rdx, 1              ; number of bytes to write

    syscall                  ; Invoke syscall to write the character

    add rsp, 8              ; Clean up the stack
    mov rax, 1              ; Return the number of bytes written
    ret                      ; Return from function
