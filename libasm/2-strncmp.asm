BITS 64

section .text
    global asm_strncmp        ; Make the function globally accessible

asm_strncmp:
    ; Function prototype: int asm_strncmp(const char *s1, const char *s2, size_t n)
    ; Arguments:
    ; rdi = s1 (first string)
    ; rsi = s2 (second string)
    ; rdx = n (number of characters to compare)

    ; Setting up the stack frame
    push    rbp              ; Save the base pointer
    mov     rbp, rsp         ; Establish the stack frame

    ; Initialize loop counter
    xor     rax, rax         ; Clear rax for the return value (default 0)

.loop:
    ; Check if we have compared 'n' characters
    cmp     rax, rdx         ; Compare loop counter with n
    jge     .done             ; If we've reached n, exit the loop

    ; Load the next byte of s1 and s2 into al and bl
    mov     al, [rdi + rax]   ; Load next byte from s1
    mov     bl, [rsi + rax]   ; Load next byte from s2

    ; Compare the bytes
    cmp     al, bl           ; Compare the bytes
    jne     .not_equal       ; If they are not equal, jump to not_equal

    ; Check for null terminator
    test    al, al           ; Check if we reached the end of the string
    jz      .equal           ; If al is 0, strings are equal so far

    ; Move to the next character
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