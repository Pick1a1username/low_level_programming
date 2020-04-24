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
  ; cmp byte [rdi+rax], 0x20 ; space
  ; je   .end
  cmp byte [rdi+rax], 0x9  ; tab 
  je   .end
  cmp byte [rdi+rax], 0x10 ; line break
  je   .end
  cmp byte [rdi+rax], 0x0a ; line break
  je   .end
  cmp byte [rdi+rax], 0x0d ; line break
  je   .end
  cmp byte [rdi+rax], 0xca ; I have no idea why this check should be added...
  je   .end
  
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
    xor rax, rax
    push rdi
    mov rdi, rsp
    call print_string
    pop rdi
    ret

print_newline:
    xor rax, rax
    mov rdi, 0xA
    call print_char
    ret


print_uint:
  xor rax, rax

  ; if the number is zero, print it immediately.
  cmp rdi, 0
  jne .non_zero_uint
  mov rdi, 0x30
  push rdi
  mov rdi, rsp
  call print_string
  pop rdi
  ret

.non_zero_uint:
  push rsi
  ; Create a buffer
  mov r11, 0x0000000000000000
  push r11
  push r11
  push r11
  mov rsi, rsp
  push rax
  push rbx
  push rdi
  push rdx

  mov rax, rdi
  mov rbx, 10 ; divider
  ; mov r12, 0
.loop_get_decimal_digits:
  mov rdx, 0 ; reset remainder
  ; https://www.tutorialspoint.com/assembly_programming/assembly_arithmetic_instructions.htm
  div rbx ; <- Floating point exception (core dumped)
  ; quotient is stored in rax
  ; remainder is stored in rdx
  ; mov [rsi], al ; copy the digit to the buffer
  mov [rsi], rdx; copy the digit to the buffer
  inc rsi
  cmp rax, 0
  je .print_uint_print_digits
  jmp .loop_get_decimal_digits

.print_uint_print_digits:
  pop rdx
  pop rdi
  pop rbx
  pop rax
  pop r11
  pop r13
  pop r14
  pop rsi


  push rbx; this will be used for the address storing a digit

  ; push digits to stack again
  push r14
  push r13
  push r11
  ; get the address of the beginning of the pushed digits
  mov rbx, rsp 
  mov r12, rsp ; get the one more address of the beginning of the pushed digits
  add rbx, 24

  ; get the address of the beginning of the digits(the first non-zero digit)
.loop_find_non_zero:
  cmp byte[rbx], 0    ; if the digit is not zero, start to print
  jne .loop_print_digits
  dec rbx
  jmp .loop_find_non_zero

  ; start to print.
.loop_print_digits:
  mov si, 0x30
  add si, word[rbx]

  push rsi
  mov rsi, rsp  ; argument #2 in rsi: where does the string start
  mov rdx, 1  ; argument #3 in rdx: how many bytes to write
  mov rax, 1  ; system call number should be stored in rax
  mov rdi, 1  ; argument #1 in rdi: where to write (descriptor)?
  push rcx
  syscall
  pop rcx
  pop rsi

  cmp rbx, r12
  je .finish_print_digits
  dec rbx
  jmp .loop_print_digits


  .finish_print_digits:
  pop rbx
  pop r11
  pop r13
  pop r14
  pop rbx

  ret



print_int:
  xor rax, rax

  ; Get the sign
  push rdi
  shr rdi, 63
  mov r15, rdi  ; r15 will hold the sign.
  pop rdi
  

  ; if the number is zero, print it immediately.
  cmp rdi, 0
  jne .print_int_non_zero_uint
  mov rdi, 0x30
  push rdi
  mov rdi, rsp
  call print_string
  pop rdi
  ret

.print_int_non_zero_uint:
  push rsi
  ; Create a buffer
  mov r11, 0x0000000000000000
  push r11
  push r11
  push r11
  mov rsi, rsp
  push rax
  push rbx
  push rdi
  push rdx

  ; divide the number by 10...
  mov rbx, 10 ; divider
  mov rax, rdi
  ; if the number is negative, remove sign and convert to positive.
  cmp r15, 0
  je .print_int_loop_get_decimal_digits
  not rax
  add rax, 1
.print_int_loop_get_decimal_digits:
  mov rdx, 0 ; reset remainder
  ; https://www.tutorialspoint.com/assembly_programming/assembly_arithmetic_instructions.htm
  div rbx ; <- Floating point exception (core dumped)
  ; quotient is stored in rax
  ; remainder is stored in rdx
  ; mov [rsi], al ; copy the digit to the buffer
  mov [rsi], rdx; copy the digit to the buffer
  inc rsi
  cmp rax, 0
  je .print_int_print_digits
  jmp .print_int_loop_get_decimal_digits

.print_int_print_digits:
  pop rdx
  pop rdi
  pop rbx
  pop rax
  pop r11
  pop r13
  pop r14
  pop rsi


  push rbx; this will be used for the address storing a digit

  ; push digits to stack again
  push r14
  push r13
  push r11

  


  ; get the address of the beginning of the pushed digits
  mov rbx, rsp 
  mov r12, rsp ; get the one more address of the beginning of the pushed digits
  add rbx, 24

  ; Print '-' at first if it's negative
  cmp r15, 0
  je .print_int_loop_find_non_zero

  push rdi
  mov rdi, 0x2d
  push rdi
  mov rdi, rsp
  call print_string
  pop rdi
  pop rdi
  
  ; get the address of the beginning of the digits(the first non-zero digit)
.print_int_loop_find_non_zero:
  cmp byte[rbx], 0    ; if the digit is not zero, start to print
  jne .print_int_loop_print_digits
  dec rbx
  jmp .print_int_loop_find_non_zero

  ; start to print.
.print_int_loop_print_digits:
  mov si, 0x30
  add si, word[rbx]

  push rsi
  mov rsi, rsp  ; argument #2 in rsi: where does the string start
  mov rdx, 1  ; argument #3 in rdx: how many bytes to write
  mov rax, 1  ; system call number should be stored in rax
  mov rdi, 1  ; argument #1 in rdi: where to write (descriptor)?
  push rcx
  syscall
  pop rcx
  pop rsi

  cmp rbx, r12
  je .print_int_finish_print_digits
  dec rbx
  jmp .print_int_loop_print_digits


.print_int_finish_print_digits:
  pop rbx
  pop r11
  pop r13
  pop r14
  pop rbx

  ret

string_equals:
    xor rax, rax
    ret


; read_char:
;     xor rax, rax
;     mov  rdx, 2       ;argument #3 in rdx: how many bytes to read
;     mov  rsi, 2  ;argument #2 in rsi: buffer
;     push rdi
;     mov  rdi, 0        ;argument #1 in rdi: where to read (descriptor)?
;     mov  rax, 0        ;system call number should be stored in rax
;     push rcx           ; syscall will break rcx
;     syscall            ;this instruction invokes a system call
;     pop rcx
;     pop rdi
;     ret 
read_char:
    push 0
    xor rax, rax
    xor rdi, rdi
    mov rsi, rsp 
    mov rdx, 1
    syscall
    pop rax
    ret 

; arguments
; rdi: buffer
; rsi: size
read_word:
   
    push rdi
    push rsi
    push rdx
    push rcx
    
    ; mov dword[rdi], 0x0000000000000000

    mov rdx, rsi ; argument #3; how many bytes to read
    mov rsi, rdi ; argument #2: buffer
    xor rax, rax
    xor rdi, rdi ; argument #1; where to read (descriptor)
    syscall

    mov rax, rsi

    pop rcx
    pop rdx
    pop rsi
    pop rdi
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
