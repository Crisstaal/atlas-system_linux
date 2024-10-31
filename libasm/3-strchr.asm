=BITS 64

section .text
    global asm_strchr        ; Make the function globally accessible

asm_strchr:
    ; Setting up the stack frame
    push    rbp               ; Save the base pointer
    mov     rbp, rsp          ; Establish the stack frame

    mov     rdi, rdi          ; The pointer to the string (s)
    mov     al, dil           ; Move the character (c) into al

.loop:
    cmp     byte [rdi], 0     ; Compare the current character to null terminator
    je      .not_found        ; If it's null, we didn't find the character
    cmp     byte [rdi], al    ; Compare the current character to the target character
    je      .found            ; If they are equal, jump to found
    inc     rdi               ; Move to the next character in the string
    jmp     .loop             ; Repeat the loop

.found:
    mov     rax, rdi          ; Set the return value to the address of the found character
    jmp     .done             ; Jump to done

.not_found:
    xor     rax, rax          ; Return NULL (0) if not found

.done:
    ; Teardown the stack frame
    mov     rsp, rbp          ; Restore the stack pointer
    pop     rbp               ; Restore the base pointer
    ret                       ; Return from the function