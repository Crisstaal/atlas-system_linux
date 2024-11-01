BITS 64 ; Indicates 64-bit architecture

section .data
    ; No global variables used so this can be blank

section .text
    global asm_strpbrk    ; Make the function globally accessible

asm_strpbrk:  ; Label for the function
    ; Setting up the stack frame
    push    rbp                 ; Preserve base pointer on the stack
    mov     rbp, rsp            ; Set the base pointer to the current stack pointer

    xor     rax, rax            ; Initialize counter for s

.s_loop:
    mov     al, byte [rdi + rax] ; Load character from s
    test    al, al               ; Check if null (end of string)
    jz      .not_found           ; If null, character not found

    ; Initialize j counter for accept
    xor     rcx, rcx            ; rcx will serve as index for accept

.accept_loop:
    mov     bl, byte [rsi + rcx] ; Load character from accept
    test    bl, bl               ; Check if null (end of accept)
    jz      .not_found           ; If null, character not found in accept

    cmp     al, bl               ; Compare character from s with accept character
    je      .found               ; If equal, character is found

    inc     rcx                  ; Move to the next character in accept
    jmp     .accept_loop         ; Repeat the check

.found:
    lea     rax, [rdi + rax]     ; Load address of found character into rax
    jmp     .done                ; Go to done

.not_found:
    inc     rax                  ; Increment s counter
    jmp     .s_loop              ; Repeat the loop

.done:
    ; Tearing down the stack frame (always do this!)
    mov     rsp, rbp            ; Restore previous stack pointer
    pop     rbp                 ; Restore previous base pointer
    ret                         ; Return from the function