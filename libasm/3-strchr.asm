BITS 64

section .text
global asm_strchr

asm_strchr:
    ; Arguments:
    ; rdi = const char *s1
    ; rsi = int c

    xor rax, rax                ; Clear rax (used for pointer return)
    test rdi, rdi               ; Check if s1 is NULL
    jz .found_null               ; If s1 is NULL, return NULL

.loop:
    mov al, byte [rdi]          ; Load byte from s1
    cmp al, sil                 ; Compare with character c
    je .found                   ; If equal, jump to found
    test al, al                 ; Check for null terminator
    jz .found_null              ; If we reach the end, return NULL
    inc rdi                     ; Move to the next character
    jmp .loop                   ; Repeat

.found:
    mov rax, rdi                ; Set rax to the address of the found character
    ret

.found_null:
    xor rax, rax                ; Return NULL
    ret
