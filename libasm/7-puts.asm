BITS 64

extern my_strlen       ; Import my_strlen from 0-strlen.asm
section .text
    global asm_puts

asm_puts:
    ; Prototype: size_t asm_puts(const char *str);

    ; Save the string address from the first argument
    mov     rsi, rdi         ; rsi now points to the input string

    ; Calculate the length of the string by calling my_strlen
    call    my_strlen        ; rax will have the string length returned

    ; Prepare for syscall
    mov     rax, 1           ; syscall number for sys_write
    mov     rdi, 1           ; file descriptor 1 is stdout
    mov     rdx, rax         ; set the third argument for the syscall (length of string)

    ; Write the string to stdout
    mov     rsi, rdi         ; point rsi to the original string again for writing
    syscall                  ; invoke the kernel

    ; Return the number of bytes written (length of string)
    ret                      ; rax already contains the string length