BITS 64

section .text
    global asm_strncmp        ; Make the function globally accessible

asm_strncmp:
    ; Function prototype: int asm_strncmp(const char *s1, const char *s2, size_t n)
    ; Arguments:
    ; rdi = s1 (first string)
    ; rsi = s2 (second string)
    ; rdx = n (number of characters to compare)

    push    rbp              ; Save the base pointer
    mov     rbp, rsp         ; Establish the stack frame

    xor     rax, rax         ; Clear rax for the loop counter

.loop:
    cmp     rax, rdx         ; Compare loop counter with n
    jge     .done             ; If we've reached n, exit the loop

    mov     al, [rdi + rax]   ; Load next byte from s1
    mov     bl, [rsi + rax]   ; Load next byte from s2

    ; Check for null terminator in either string
    test    al, al            ; Check if the character from s1 is null
    jz      .equal            ; If s1 is null, treat as equal if s2 is also null
    test    bl, bl            ; Check if the character from s2 is null
    jz      .not_equal        ; If s2 is null, s1 is greater if al is not null

    cmp     al, bl           ; Compare the bytes
    jne     .not_equal       ; If they are not equal, jump to not_equal

    inc     rax              ; Increment the loop counter
    jmp     .loop            ; Repeat the loop

.not_equal:
    ; Zero-extend al and bl to 32-bit registers for safe subtraction
    movzx   eax, al          ; Move al into eax, zero-extending it
    movzx   ebx, bl          ; Move bl into ebx, zero-extending it
    sub     eax, ebx         ; Compute the difference in eax
    movsx   rax, eax         ; Sign-extend eax into rax for the return value
    jmp     .done            ; Jump to done

.equal:
    xor     rax, rax         ; Set return value to 0 for equal strings

.done:
    mov     rsp, rbp         ; Restore the stack pointer
    pop     rbp              ; Restore the base pointer
    ret                       ; Return from the function