BITS 64

section .text
    global asm_strcmp        ; Make the function globally accessible

asm_strcmp:
    ; Setting up the stack frame
    push    rbp              ; Save the base pointer
    mov     rbp, rsp         ; Establish the stack frame

    ; Loop through both strings
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
    sub     rax, rax         ; Clear rax
    mov     rax, al          ; Move the value of al (character from s1) into rax
    sub     rax, bl          ; Subtract the character from s2
    jmp     .done            ; Jump to done

.equal:
    xor     rax, rax         ; Set return value to 0 for equal strings

.done:
    ; Teardown the stack frame
    mov     rsp, rbp         ; Restore the stack pointer
    pop     rbp              ; Restore the base pointer
    ret                      ; Return from the function