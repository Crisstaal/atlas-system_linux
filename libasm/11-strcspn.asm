BITS 64 ; Indicates 64-bit architecture

section .data
    ; No global variables used so this can be blank

section .text
    global asm_strcspn    ; Make the function globally accessible

asm_strcspn:  ; Label for the function
    ; Setting up the stack frame
    push    rbp                 ; Preserve base pointer on the stack
    mov     rbp, rsp            ; Set the base pointer to the current stack pointer

    xor     rax, rax            ; Initialize counter (i = 0)

.s_loop:
    mov     al, byte [rdi + rax] ; Load character from s
    test    al, al               ; Check if null (end of string)
    jz      .done                ; If null, we're done

    ; Check if the character is in the reject string
    push    rsi                  ; Save reject string pointer
    call    .is_char_in_reject   ; Call helper function
    pop     rsi                  ; Restore reject string pointer

    test    al, al               ; Check if character was found in reject
    jnz     .done                ; If found, we're done

    inc     rax                  ; Increment the count 
    jmp     .s_loop              ; Repeat the loop

.done:
    ; Tearing down the stack frame
    mov     rsp, rbp            ; Restore previous stack pointer
    pop     rbp                 ; Restore previous base pointer
    ret                         ; Return from the function

; Helper function: Check if the character in al is in the reject string
.is_char_in_reject:
    xor     rcx, rcx            ; j counter

.check_loop:
    mov     bl, byte [rsi + rcx] ; Load character from reject
    test    bl, bl               ; Check if null (end of reject)
    jz      .not_found           ; If null, character not found

    cmp     al, bl               ; Compare character from s with reject character
    je      .found               ; If equal, character is found

    inc     rcx                  ; Move to the next character in reject
    jmp     .check_loop          ; Repeat the check

.not_found:
    xor     al, al               ; Set return value to 0 (false)
    ret

.found:
    mov     al, 1                ; Set return value to 1 (true)
    ret