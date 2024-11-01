BITS 64

global asm_strcmp            ; Make the function globally accessible
section .text

asm_strcmp:
    ; Setting up the stack frame
    push    rbp              ; Save the base pointer
    mov     rbp, rsp         ; Establish the stack frame

.loop:
    mov     al, [rdi]        ; Load the next byte of s1 into al
    mov     bl, [rsi]        ; Load the next byte of s2 into bl
    cmp     al, bl           ; Compare the bytes
    jne     .not_equal       ; If they are not equal, jump to not_equal
    test    al, al           ; Check if we reached the end of the string (null terminator)
    jz      .equal           ; If both are null terminators, they are equal

    inc     rdi              ; Move to the next character in s1
    inc     rsi              ; Move to the next character in s2
    jmp     .loop            ; Repeat the loop

.not_equal:
    movzx   eax, al          ; Zero-extend al into eax
    movzx   ebx, bl          ; Zero-extend bl into ebx
    sub     eax, ebx         ; Compute the difference in eax (result in eax)

.done:
    mov     rsp, rbp         ; Restore the stack pointer
    pop     rbp              ; Restore the base pointer
    ret                      ; Return from the function

.equal:
    xor     eax, eax         ; Set return value to 0 for equal strings
    jmp     .done            ; Jump to done