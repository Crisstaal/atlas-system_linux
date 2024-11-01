BITS 64

section .text
    global asm_strchr        ; Make the function globally accessible

asm_strchr:
    ; Input: rdi = pointer to string (const char *s), rsi = character (int c)
    
    ; Check if the string is NULL
    test rdi, rdi
    jz .not_found       ; If NULL, go to not_found

.loop:
    ; Load the current character from the string
    mov al, [rdi]      ; Get the character at the current position
    cmp al, 0          ; Check if it's the end of the string
    je .not_found       ; If it is, go to not_found
    
    cmp al, sil        ; Compare with the character we are searching for
    je .found          ; If found, jump to found
    
    ; Move to the next character
    inc rdi            ; Increment pointer to the next character
    jmp .loop          ; Repeat the loop

.found:
    mov rax, rdi       ; Return the pointer to the found character
    ret

.not_found:
    xor rax, rax       ; Return NULL (0) if not found
    ret