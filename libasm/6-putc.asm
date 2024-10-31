BITS 64

section .text
    global asm_putc

asm_putc:
    ; Prototype: size_t asm_putc(int c);

    ; Prepare for syscall
    mov     rax, 1          ; syscall number for sys_write
    mov     rdi, 1          ; file descriptor 1 is stdout
    mov     rsi, rsp        ; Use rsp to store the address of the character
    mov     byte [rsp], al  ; Store the character in memory at rsp

    ; Set the third argument for syscall (size_t count)
    mov     rdx, 1          ; we want to write 1 byte

    syscall                 ; invoke the kernel

    ; Return the number of bytes written (1 in this case)
    mov     rax, 1          ; set rax to 1 to indicate 1 byte written
    ret                     ; return