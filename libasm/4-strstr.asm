BITS 64

section .text
global asm_strstr

section .text
global asm_strstr

; char *asm_strstr(const char *haystack, const char *needle);
asm_strstr:
    ; Check if needle is NULL
    test rsi, rsi           ; Check if needle is NULL
    jz .found                ; If needle is NULL, return haystack

    ; Check if needle is empty
    cmp byte [rsi], 0       ; Check if needle is empty
    je .found                ; If empty, return haystack

    ; Check if haystack is NULL
    test rdi, rdi           ; Check if haystack is NULL
    jz .not_found            ; If haystack is NULL, return NULL

.next_char:
    ; Save haystack pointer
    mov rax, rdi             ; rax = haystack

    ; Check for the end of haystack
    mov rcx, rsi             ; rcx = needle
    mov rdx, rax             ; rdx = current position in haystack

.check_needle:
    ; Compare characters
    mov al, [rdx]           ; Load character from haystack
    mov bl, [rcx]           ; Load character from needle
    test al, al              ; Check if end of haystack
    jz .not_found            ; If end of haystack, needle not found
    cmp al, bl              ; Compare characters
    jne .next_char_2        ; If not equal, move to the next char in haystack

    ; Move to the next character in needle
    inc rcx                  ; Increment needle pointer
    inc rdx                  ; Increment haystack pointer
    cmp byte [rcx], 0       ; Check if end of needle
    jne .check_needle        ; If not end of needle, continue checking

.found:
    mov rax, rdi             ; Return the original haystack pointer
    ret

.next_char_2:
    inc rdi                  ; Move to the next character in haystack
    jmp .next_char           ; Check again

.not_found:
    xor rax, rax             ; Return NULL
    ret
