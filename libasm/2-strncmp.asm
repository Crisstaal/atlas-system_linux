BITS 64

section .text
    global asm_strncmp        ; Make the function globally accessible

asm_strncmp:
    ; Setting up the stack frame
    push    rbp               ; Save the base pointer
    mov     rbp, rsp          ; Establish the stack frame

    ; Initialize counter for n
    mov     rcx, rdx          ; Move n (third argument) into rcx for the loop counter

.loop:
    test    rcx, rcx          ; Check if n is 0
    jz      .done             ; If n is 0, jump to done

    mov     al, [rdi]         ; Load the next byte of s1 into al
    mov     bl, [rsi]         ; Load the next byte of s2 into bl
    cmp     al, bl            ; Compare the bytes
    jne     .not_equal        ; If they are not equal, jump to not_equal

    test    al, al            ; Check if we reached the end of the string (null terminator)
    jz      .equal            ; If null terminator is reached, they are considered equal

    inc     rdi               ; Move to the next character in s1
    inc     rsi               ; Move to the next character in s2
    dec     rcx               ; Decrease the counter
    jmp     .loop             ; Repeat the loop

.not_equal:
    sub     rax, rax          ; Clear rax
    mov     rax, al           ; Move the character from s1 into rax
    sub     rax, bl           ; Subtract the character from s2
    jmp     .done             ; Jump to done

.equal:
    xor     rax, rax          ; Set return value to 0 for equal strings

.done:
    ; Teardown the stack frame
    mov     rsp, rbp          ; Restore the stack pointer
    pop     rbp               ; Restore the base pointer
    ret                       ; Return from the function