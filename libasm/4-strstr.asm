BITS 64

section .text
    global asm_strstr        ; Make the function globally accessible

asm_strstr:
    ; Setting up the stack frame
    push    rbp               ; Save the base pointer
    mov     rbp, rsp          ; Establish the stack frame

    mov     rdi, rdi          ; The pointer to the haystack
    mov     rsi, rsi          ; The pointer to the needle

    ; Handle the case where needle is empty
    test    rsi, rsi          ; Check if needle is NULL
    jz      .done             ; If needle is NULL, return haystack

.loop_haystack:               ; Loop through the haystack
    cmp     byte [rdi], 0     ; Check if end of haystack
    je      .not_found        ; If it's null, not found

    ; Store current haystack position
    mov     rdx, rdi          ; rdx holds current position in haystack
    mov     rcx, rsi          ; rcx holds current position in needle

.loop_needle:                 ; Loop through the needle
    cmp     byte [rcx], 0     ; Check if end of needle
    je      .found            ; If it's null, found the substring

    cmp     byte [rdx], byte [rcx] ; Compare characters
    jne     .next_haystack    ; If not equal, go to next character in haystack

    inc     rdx               ; Move to next character in haystack
    inc     rcx               ; Move to next character in needle
    jmp     .loop_needle      ; Repeat for next character

.next_haystack:
    inc     rdi               ; Move to next character in haystack
    jmp     .loop_haystack    ; Check the next character in haystack

.found:
    mov     rax, rdi          ; Set return value to the address of the found substring
    jmp     .done             ; Jump to done

.not_found:
    xor     rax, rax          ; Return NULL (0) if not found

.done:
    ; Teardown the stack frame
    mov     rsp, rbp          ; Restore the stack pointer
    pop     rbp               ; Restore the base pointer
    ret                       ; Return from the function