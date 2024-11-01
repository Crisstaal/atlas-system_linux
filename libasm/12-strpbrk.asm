BITS 64 ; Indicates 64-bit architecture

section .data
    ; No global variables used so this can be blank

section .text
    global asm_strpbrk    ; Make the function globally accessible

asm_strpbrk:  ; Label for the function
    ; Setting up the stack frame (always do this!)
    push    rbp                 ; Preserve base pointer on the stack
    mov     rbp, rsp            ; Set the base pointer to the current stack pointer

    xor     rax, rax            ; Initialize counter for s (i = 0)

.s_loop:
    mov     al, byte [rdi + rax] ; Load character from s
    test    al, al               ; Check if null (end of string)
    jz      .not_found           ; If null, character not found

    ; Check if the character is in the accept string
    push    rsi                  ; Save accept string pointer
    call    .is_char_in_accept   ; Call helper function
    pop     rsi                  ; Restore accept string pointer

    test    al, al               ; Check if character was found in accept
    jz      .not_found           ; If not found, continue

    ; If found, return pointer to the character in s
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

; Helper function: Check if the character in al is in the accept string
.is_char_in_accept:
    xor     rcx, rcx            ; j counter

.check_loop:
    mov     bl, byte [rsi + rcx] ; Load character from accept
    test    bl, bl               ; Check if null (end of accept)
    jz      .not_found_accept     ; If null, character not found

    cmp     al, bl               ; Compare character from s with accept character
    je      .found_accept         ; If equal, character is found

    inc     rcx                  ; Move to the next character in accept
    jmp     .check_loop          ; Repeat the check

.not_found_accept:
    xor     al, al               ; Set return value to 0 (false)
    ret

.found_accept:
    mov     al, 1                ; Set return value to 1 (true)
    ret