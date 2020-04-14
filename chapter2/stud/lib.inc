section .text

; the string will be passed to rdi.
; the length should be saved to rax.
string_length:
    xor rax, rax
.loop:                   ; main loop starts here
  cmp byte [rdi+rax], 0  ; Check if the current symbol is null-terminator.
                         ; We absolutely need that 'byte' modifier since
                         ; the left and the right part of cmp should be
                         ; of the same size. Right operand is immediate
                         ; and holds no information about its size,
                         ; hence we don't know how many bytes should be
                         ; taken from memory and compared to zero.
  je   .end              ; Jump if we found null-terminator
  inc  rax               ; Otherwise go to next symbol and increase
                         ; counter
  jmp .loop
.end:
    ret

print_string:
    xor rax, rax

; most of codes are copied from hello.asm
  push rax
  call string_length
  mov  rdx, rax       ;argument #3 in rdx: how many bytes to write
  mov  rsi, rdi  ;argument #2 in rsi: where does the string start
  push rdi
  mov  rdi, 1        ;argument #1 in rdi: where to write (descriptor)?
  mov  rax, 1        ;system call number should be stored in rax
  push rcx           ; syscall will break rcx
  syscall            ;this instruction invokes a system call
  pop rcx
  pop rdi
  pop rax
  ret

; print_char:
;     xor rax, rax
; 
;     ; most of codes are copied from hello.asm
;     mov  rdx, 64       ;argument #3 in rdx: how many bytes to write
;     mov  rsi, rdi  ;argument #2 in rsi: where does the string start
;     push rdi
;     mov  rdi, 1        ;argument #1 in rdi: where to write (descriptor)?
;     mov  rax, 1        ;system call number should be stored in rax
;     push rcx           ; syscall will break rcx
;     syscall            ;this instruction invokes a system call
;     pop rcx
;     pop rdi
; 
;     ret

print_char:
    push rdi
    mov rdi, rsp
    call print_string
    pop rdi

print_newline:
    xor rax, rax
    ret


print_uint:
    xor rax, rax
    ret


print_int:
    xor rax, rax
    ret

string_equals:
    xor rax, rax
    ret


read_char:
    xor rax, rax
    ret 

read_word:
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    ret 


; rdi: string
; rsi: buffer
; rdx: buffer's length
; string_copy:
;     xor rax, rax
; 
;     ; Check the string is smaller than the buffer.
;     push rdi
;     push rsi
;     push rdx
;     call string_length
;     pop rdi
;     pop rsi
;     pop rdx
; 
; 
;     cmp rax, rdx    ; string's length vs buffer's length
;     jbe .copy_str
; 
;     xor rax, rax      ; If the string is bigger than the buffer, return zero.
;     ret
; .copy_str:
;     ; If the buffer is enoughly big, copy the string to the buffer.
;     ; https://www.cs.virginia.edu/~evans/cs216/guides/x86.html
;     mov [rsi], rdi
;     lea rax, [rsi]
; 
;     ret
;     
;     ; rdi = source
;     ; rsi = dest
;     ; rdx = dest length 
string_copy:

    push rdi
    push rsi
    push rdx
    call string_length
    pop rdx
    pop rsi
    pop rdi

    cmp rax, rdx
    jae .too_long  ; we also need to store null-terminator
    
    push rsi 

    ; rdx edx dx dh dl
        .loop: 
        mov dl, byte[rdi]
        mov byte[rsi], dl
        inc rdi
        inc rsi
        test dl, dl
        jnz .loop 

    pop rax 
    ret

    .too_long:
    xor rax, rax
    ret
