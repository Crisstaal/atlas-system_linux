BITS 64

section .text
global asm_memcpy

section .text
global asm_memcpy

; void *asm_memcpy(void *dest, const void *src, size_t n);
asm_memcpy:
    ; Check if n is zero
    test rdx, rdx            ; Check if n is zero
    jz .done                 ; If n is zero, just return dest

    ; Move src and dest to registers
    mov rax, rdi             ; rax = dest
    mov rcx, rsi             ; rcx = src

.copy_loop:
    movsb                    ; Copy byte from src (rcx) to dest (rax)
    inc rax                  ; Increment dest pointer
    inc rcx                  ; Increment src pointer
    dec rdx                  ; Decrement n
    jnz .copy_loop           ; Repeat until n is zero

.done:
    ret                      ; Return dest