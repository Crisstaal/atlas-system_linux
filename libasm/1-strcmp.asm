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
    ; Check if one of the strings is empty
    test    al, al           ; Check if al (S1) is null
    jz      .s1_empty        ; If S1 is empty, jump to s1_empty
    test    bl, bl           ; Check if bl (S2) is null
    jz      .s2_empty        ; If S2 is empty, jump to s2_empty

    ; Calculate the difference and return
    movzx   eax, al          ; Zero-extend al into eax
    movzx   ebx, bl          ; Zero-extend bl into ebx
    sub     eax, ebx         ; Compute the difference in eax
    jmp     .done            ; Jump to done

.s1_empty:
    ; If S1 is empty, return -bl (negative of bl)
    movzx   eax, bl          ; Move bl into eax
    neg     eax              ; Negate it to make it negative
    jmp     .done            ; Jump to done

.s2_empty:
    ; If S2 is empty, return al (positive value)
    movzx   eax, al          ; Move al into eax
    jmp     .done            ; Jump to done

.equal:
    xor     eax, eax         ; Set return value to 0 for equal strings

.done:
    mov     rsp, rbp         ; Restore the stack pointer
    pop     rbp              ; Restore the base pointer
    ret                      ; Return from the function