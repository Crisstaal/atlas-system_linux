BITS 64

section .text
    global my_strlen  ; Make the function accessible to other files

my_strlen:
    ; Prototype: size_t my_strlen(const char *str);
    
    xor     rcx, rcx          ; Zero out rcx (this will be our counter)
    
    .loop:
        mov     al, [rdi + rcx] ; Load the byte at str[rcx] into al
        test    al, al          ; Test if it's the null terminator
        jz      .done           ; If it is, jump to done
        inc     rcx             ; Increment counter
        jmp     .loop           ; Repeat the loop

    .done:
        mov     rax, rcx        ; Move the counter (length) to rax
        ret                     ; Return