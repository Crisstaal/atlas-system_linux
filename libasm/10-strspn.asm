BITS 64 ; Indicates 64-bit architecture

section .data
    ; No global variables used so this can be blank

section .text
    global asm_strspn    ; Make the function globally accessible

asm_strspn:  ; Label for the function
    ; Setting up the stack frame (always do this!)
    push    rbp                 ; Preserve base pointer on the stack
    mov     rbp, rsp            ; Set the base pointer to the current stack pointer

    xor     rax, rax            ; i counter

.s_loop:
    mov     al, byte [rdi+rax]
    test    al, al
    jz      .done

.accept_loop:
    xor     rcx, rcx            ; j counter
    mov     bl, byte [rsi+rcx]
    test    bl, bl              ; check for null-term
    jz      .done
    cmp     al, bl
    je      .next_outer
    inc     rcx
    jmp     .accept_loop

.next_outer:
    inc     rax
    jmp     .s_loop

.done:
    ; Tearing down the stack frame (always do this!)
    mov     rsp, rbp            ; Restore previous stack pointer
    pop     rbp                 ; Restore previous base pointer
    ret                         ; Return from the function