  
global find_word
extern string_equals

section .rodata
msg_noword: db "No such word",0

section .text

; * Arguments
;   * rdi: A pointer to a null terminated key string.
;   * rsi: A label to the last word in the dictionary.
; * Returns
;   * rax: 0, if the record is not found.
;          record address, otherwise.
find_word: 
    xor rax, rax
.loop:
    test rsi, rsi 
    jz .end 
    push rdi
    push rsi
    add rsi, 8
    call string_equals
    pop rsi
    pop rdi
    test rax, rax
    jnz .found
    mov rsi, [rsi]
    jmp .loop
.found:
    mov rax, rsi
.end:
    ret
