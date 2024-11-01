BITS 64

section .text
    global asm_strcasecmp  ; Make the function accessible to other files

asm_strcasecmp:
    ; Prototype: int asm_strcasecmp(const char *s1, const char *s2);
    
    xor     rax, rax          ; Clear rax (this will hold the return value)
    
.loop:
    mov     al, [rdi]         ; Load the next byte from s1 into al
    mov     bl, [rsi]         ; Load the next byte from s2 into bl
    
    ; Convert to lowercase if necessary
    ; Check if the character is uppercase and convert it to lowercase
    cmp     al, 'A'
    jb      .check_end        ; If al < 'A', skip to end check
    cmp     al, 'Z'
    ja      .check_end        ; If al > 'Z', skip to end check
    add     al, 32            ; Convert uppercase to lowercase

.check_end:
    ; Check the end of both strings
    cmp     al, 0
    je      .done             ; If we hit the null terminator in s1, we're done
    cmp     bl, 0
    je      .done             ; If we hit the null terminator in s2, we're done
    
    ; Convert bl to lowercase if necessary
    cmp     bl, 'A'
    jb      .compare          ; If bl < 'A', skip to comparison
    cmp     bl, 'Z'
    ja      .compare          ; If bl > 'Z', skip to comparison
    add     bl, 32            ; Convert uppercase to lowercase

.compare:
    ; Compare the characters
    cmp     al, bl
    jne     .different         ; If they are different, jump to different handling
    
    ; Move to the next character
    inc     rdi               ; Move to the next character in s1
    inc     rsi               ; Move to the next character in s2
    jmp     .loop             ; Repeat the loop

.different:
    ; If characters are different, return the difference
    sub     rax, rax          ; Clear rax
    sub     al, bl            ; Calculate the difference
    ret                       ; Return the result

.done:
    xor     rax, rax          ; If we reach the end of both strings, return 0
    ret                       ; Return 0
