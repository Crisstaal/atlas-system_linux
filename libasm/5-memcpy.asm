BITS 64

section .text
    global asm_memcpy

asm_memcpy:
    ; Prototypes: void *asm_memcpy(void *dest, const void *src, size_t n);
    
    push    rbp             ; Save the base pointer
    mov     rbp, rsp        ; Set the base pointer to the current stack pointer

    ; Check if n is zero, if yes, return dest
    test    rdx, rdx        ; Check if n (size) is 0
    jz      .done           ; If zero, return dest

    ; Move src and dest to registers for ease of use
    mov     rax, rdi        ; Move dest to rax
    mov     rcx, rsi        ; Move src to rcx

    ; Copy n bytes from src to dest
.copy_loop:
    mov     al, [rcx]       ; Load byte from src into al
    mov     [rax], al       ; Store byte into dest
    inc     rax             ; Move to the next byte in dest
    inc     rcx             ; Move to the next byte in src
    dec     rdx             ; Decrement the count
    jnz     .copy_loop      ; Repeat until n bytes are copied

.done:
    mov     rsp, rbp        ; Restore stack pointer
    pop     rbp             ; Restore base pointer
    ret                     ; Return dest pointer