BITS 64

section .text
    global asm_strncasecmp  ; Make the function accessible to other files

asm_strncasecmp:
    ; Prototype: int asm_strncasecmp(const char *s1, const char *s2, size_t n);
    
    xor     rax, rax           ; Clear rax (this will hold the return value)

.loop:
    test    rdx, rdx           ; Check if n is zero
    jz      .done               ; If n is zero, we're done
    
    mov     al, [rdi]          ; Load the next byte from s1 into al
    mov     bl, [rsi]          ; Load the next byte from s2 into bl
    
    ; Convert to lowercase if necessary for s1
    cmp     al, 'A'
    jb      .check_s2          ; If al < 'A', skip to s2 check
    cmp     al, 'Z'
    ja      .check_s2          ; If al > 'Z', skip to s2 check
    add     al, 32             ; Convert uppercase to lowercase

.check_s2:
    ; Convert to lowercase if necessary for s2
    cmp     bl, 'A'
    jb      .compare            ; If bl < 'A', skip to comparison
    cmp     bl, 'Z'
    ja      .compare            ; If bl > 'Z', skip to comparison
    add     bl, 32              ; Convert uppercase to lowercase

.compare:
    ; Compare the characters
    cmp     al, bl
    jne     .different           ; If they are different, jump to different handling
    
    ; Move to the next character
    inc     rdi                ; Move to the next character in s1
    inc     rsi                ; Move to the next character in s2
    dec     rdx                ; Decrement n
    jmp     .loop              ; Repeat the loop

.different:
    ; If characters are different, return the difference
    sub     rax, rax           ; Clear rax
    sub     al, bl             ; Calculate the difference
    ret                        ; Return the result

.done:
    xor     rax, rax           ; If we reach the end of the strings and n is exhausted, return 0
    ret                        ; Return 0