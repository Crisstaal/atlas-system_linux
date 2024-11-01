BITS 64 ; Indicates 64-bit architecture

section .data
    ; No global variables used so this can be blank

section .text
    global asm_strcspn    ; Make the function globally accessible

asm_strcspn:  ; Label for the function
    xor     rax, rax            ; Initialize counter (i = 0)

.s_loop:
    mov     al, byte [rdi + rax] ; Load character from s
    test    al, al               ; Check if null
    jz      .done                ; If null, we're done

    ; Check if the character is in the reject string
    xor     rcx, rcx            ; Reset index for reject string

.check_loop:
    mov     bl, byte [rsi + rcx] ; Load character from reject
    test    bl, bl               ; Check if null (end of reject)
    jz      .not_found           ; If null, character not found

    cmp     al, bl               ; Compare character from s with reject character
    je      .done                ; If equal, character is found

    inc     rcx                  ; Move to the next character in reject
    jmp     .check_loop          ; Repeat the check

.not_found:
    inc     rax                  ; Increment the count
    jmp     .s_loop              ; Go back to the main loop

.done:
    ret                          ; Return with the count in rax
