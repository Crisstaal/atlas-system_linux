BITS 64

    global my_strlen  ; Make the function accessible to other files
    section .text
my_strlen:
    ; Prototype: size_t my_strlen(const char *str);

    xor     rcx, rcx          ; Zero out rcx

    .loop:
        mov     al, [rdi + rcx] ; Load the byte at str[rcx] into al
        test    al, al          ; Test if it's the null terminator
        
        add     rcx, 1            ; Increment counter
        cmp     al, 0              ; Compare al with 0
        jne     .loop              ; If not zero, repeat the loop
        
        ; Done
        mov     rax, rcx          ; Move the counter (length) to rax
        ret                        ; Return
