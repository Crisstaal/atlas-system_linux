BITS 64

section .text
    global asm_strncmp        ; Make the function globally accessible

asm_strncmp:
    ; Input: rdi = s1, rsi = s2, rdx = n
    ; Save registers, initialize counters, etc.
    
.loop:
    cmp rdx, 0          ; Check if n is 0
    je .equal           ; If yes, return 0 (equal)
    
    mov al, [rdi]      ; Load the byte from s1
    mov bl, [rsi]      ; Load the byte from s2
    cmp al, bl         ; Compare the two characters
    jne .not_equal      ; If not equal, jump to not_equal
    
    ; If equal, advance the pointers
    inc rdi
    inc rsi
    dec rdx            ; Decrease n
    jmp .loop          ; Repeat

.not_equal:
    sub al, bl         ; Calculate the difference
    movzx rax, al      ; Zero-extend the result
    ret

.equal:
    xor rax, rax       ; Return 0
    ret