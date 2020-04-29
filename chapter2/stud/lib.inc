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
  ; cmp byte [rdi+rax], 0x9  ; tab 
  ; je   .end
  ; cmp byte [rdi+rax], 0x10 ; line break
  ; je   .end
  ; cmp byte [rdi+rax], 0x0a ; line break
  ; je   .end
  ; cmp byte [rdi+rax], 0x0d ; line break
  ; je   .end
  ; cmp byte [rdi+rax], 0xca ; I have no idea why this check should be added...
  ; je   .end
  
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


; arguments:
; - rdi: the address of string
; returns:
; - rax: the number parsed.
; - rdx: the length of the number when it is represented by decimal.
print_uint:
  xor rax, rax
  ; Push some registers to stack which will be used in this function.
  push r11
  push r12
  push r13
  push r14 ; temporary character to print
  push r15 ; counter 
  push rsi ; The buffer's address
  push rbx ; This register is used by div instruction.


  ; Create a buffer.
  push 0
  push 0
  push 0
  mov rsi, rsp ; Get the address of the buffer.


  ; Divide the number by 10 in order to convert hexdecimal to decimal.
  xor r15, r15 ; counter for length?
  mov rax, rdi
  mov rbx, 10 ; divider
.print_uint_loop_get_digits:
  xor rdx, rdx ; reset remainder
  ; https://www.tutorialspoint.com/assembly_programming/assembly_arithmetic_instructions.htm
  div rbx ; quotient is stored in rax
          ; remainder is stored in rdx
  mov [rsi+r15], rdx; copy the digit to the buffer
  cmp rax, 0
  je .print_uint_loop_get_digits_finish

  inc r15
  jmp .print_uint_loop_get_digits


.print_uint_loop_get_digits_finish:
  ; store the length
  mov rdx, r15
  inc rdx


  ; Start to print each digit until refering the beginning of the buffer.
.print_uint_loop_print:
  ; Prepare a digit to print
  mov r14, [rsi+r15]
  and r14, 0x00000000000000FF
  add r14, 0x30

  ; Print the digit.
  push rsi ; Todo: find why rsi should be backed up when using print_char.
  push rdi
  mov rdi, r14
  call print_char
  pop rdi
  pop rsi
  
  test r15, r15
  jz .print_uint_finish

  dec r15
  jmp .print_uint_loop_print


.print_uint_finish:
  ; Clear the buffer
  pop rbx
  pop rbx
  pop rbx
  xor rbx, rbx

  ; Pop data in stack to some registers which will be used in this function.
  pop rbx
  pop rsi
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  ret


; arguments:
; - rdi: the address of string
; returns:
; - rax: the number parsed.
; - rdx: the length of the number when it is represented by decimal.
print_int:
  xor rax, rax
  ; Push some registers to stack which will be used in this function.
  push r11
  push r12
  push r13 ; sign
  push r14 ; temporary character to print
  push r15 ; counter 
  push rsi ; The buffer's address
  push rbx ; This register is used by div instruction.

  ; Check whether it's negative.
  push rdi
  shr rdi, 63
  mov r13, rdi 
  pop rdi

  ; Create a buffer.
  push 0
  push 0
  push 0
  mov rsi, rsp ; Get the address of the buffer.


  ; Divide the number by 10 in order to convert hexdecimal to decimal.
  xor r15, r15 ; counter for length?
  mov rax, rdi ; the number will be divided.
  mov rbx, 10 ; divider

  ; if the number is negative, remove sign and convert to positive.
  test r13, r13
  jz .print_int_loop_get_digits
  not rax
  add rax, 1

.print_int_loop_get_digits:
  xor rdx, rdx ; reset remainder
  ; https://www.tutorialspoint.com/assembly_programming/assembly_arithmetic_instructions.htm
  div rbx ; quotient is stored in rax
          ; remainder is stored in rdx
  mov [rsi+r15], rdx; copy the digit to the buffer
  cmp rax, 0
  je .print_int_loop_get_digits_finish

  inc r15
  jmp .print_int_loop_get_digits


.print_int_loop_get_digits_finish:
  ; store the length
  mov rdx, r15
  inc rdx


  ; Start to print each digit until refering the beginning of the buffer.


  ; if the number is negative, print minus sign at first.
  test r13, r13
  jz .print_int_loop_print

  push rsi ; Todo: find why rsi should be backed up when using print_char.
  push rdi
  mov rdi, 0x2d
  call print_char
  pop rdi
  pop rsi


.print_int_loop_print:
  ; Prepare a digit to print
  mov r14, [rsi+r15]
  and r14, 0x00000000000000FF
  add r14, 0x30

  ; Print the digit.
  push rsi ; Todo: find why rsi should be backed up when using print_char.
  push rdi
  mov rdi, r14
  call print_char
  pop rdi
  pop rsi
  
  test r15, r15
  jz .print_int_finish

  dec r15
  jmp .print_int_loop_print


.print_int_finish:
  ; Clear the buffer
  pop rbx
  pop rbx
  pop rbx
  xor rbx, rbx

  ; Pop data in stack to some registers which will be used in this function.
  pop rbx
  pop rsi
  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  ret

; arguments:
; * rdi: string 1
; * rsi: string 2
; returns:
; * rax: 1 if it's equal, 0 if it's not equal
string_equals:
    xor rax, rax
    ; Back up some registers.
    push r15 ; will be used as counter.
    push r14 ; will be used as temporary container for string 1
    push r13 ; will be used as temporary container for string 2
    
    xor r15, r15


.string_equals_loop_compare:
    mov r14, [rdi+r15]
    and r14, 0x00000000000000FF
    mov r13, [rsi+r15]
    and r13, 0x00000000000000FF
    
    cmp r14, r13 ; check whether it's equal.
    jne .string_equals_not_equal

    test r13, r13 ; check whether it's the end.
    jz .string_equals_equal

    inc r15
    jmp .string_equals_loop_compare

    
.string_equals_equal:
    mov rax, 1
    jmp .string_equals_finish

.string_equals_not_equal:
    mov rax, 0
    jmp .string_equals_finish

.string_equals_finish:
    pop r13
    pop r14
    pop r15

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
    xor rax, rax  ; system call number.
    xor rdi, rdi  ; argument #1: where to read (descriptor)?
    mov rsi, rsp  ; argument #2: buffer's address
    mov rdx, 1    ; argument #3: how many bytes to read.
    syscall
    pop rax
    ret 

; arguments
; rdi: buffer's address
; rsi: size
; returns
; rax; buffer's address
; rdx; the number of characters?
; read_word:
;    
;     ; read word
;     push rdi
;     push rsi
;     push rdx
;     push rcx
;     
;     ; mov dword[rdi], 0x0000000000000000
; 
;     mov rdx, rsi ; argument #3; how many bytes to read
;     mov rsi, rdi ; argument #2: buffer
;     xor rax, rax
;     xor rdi, rdi ; argument #1; where to read (descriptor)
;     syscall
; 
;     mov rax, rsi
; 
;     pop rcx
;     pop rdx
;     pop rsi
;     pop rdi
; 
;     ; get the length of the word.
;     push rax
; 
;     mov rdi, rax
;     call string_length
;     mov rdx, rax
; 
;     pop rax
; 
;     ret


; this is the solution.
; There had not been any comment.
;
; arguments
; rdi: buffer's address
; rsi: size
; returns
; rax; buffer's address
; rdx; the number of characters. 
read_word:
    push r14
    push r15
    xor r14, r14   ; Initialize r14 to 0.
                   ; r14 is for the number of characters.
                   ; And also it is used for ... address.
    mov r15, rsi   ; r15 contains the size of buffer.
    dec r15


    ; Get user's input.
    ; This is for removing the leading whitespaces/linebreaks/tabs.
    .A: 
    push rdi
    call read_char
    pop rdi
    cmp al, ' ' ; is it whitespace?
    je .A
    cmp al, 10  ; is it line break?
    je .A
    cmp al, 13  ; is it line break(enter)???
    je .A
    cmp al, 9   ; is it tab
    je .A
    test al, al ; is it zero? is the input over?
    jz .C


    ; Get user's input after the user starts to type alphabet/number.
    .B: 
    mov byte [rdi + r14], al  ; save the character typed at the last time.
    inc r14                   ; increment 

    push rdi
    call read_char ; Get a character.
    pop rdi
    cmp al, ' ' ; is it a whitespace?
    je .C
    cmp al, 10  ; is it a line break?
    je .C
    cmp al, 13  ; is it a line break(enter)???
    je .C
    cmp al, 9   ; is it a tab?
    je .C
    test al, al ; is it zero? is the input over?
    jz .C
    cmp r14, r15 ; is the buffer full?
    je .D

    jmp .B


    ; Return the result when the input is over.
    .C: 
    mov byte [rdi + r14], 0 ; add null terminator.
    mov rax, rdi  ; return the buffer's address at RAX.

    mov rdx, r14  ; return the length of the word at RDX.
    pop r15
    pop r14
    ret

    .D:   ; Return the result when the buffer is full.
    xor rax, rax
    pop r15
    pop r14
    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
    xor rax, rax
    
    ; if the number is zero, print it immediately.
    cmp word[rdi], 0x0030
    jne .parse_uint_non_zero_uint
    mov rax, 0
    mov rdx, 1
    ret

    ; Get each character into number.
    .parse_uint_non_zero_uint:
    push rsi
    push r15
    push r14
    push r13
    xor r15, r15  ; r15 will be used as counter
    xor r14, r14  ; r14 will be used as temporary store for digit
    xor r13, r13  ; r13 will be used as another counter

    ; create buffer
    push 0
    push 0
    push 0
    mov rsi, rsp

    .parse_uint_loop_save_digit_start:
    ; save each digit to the buffer.
    mov r14, [rdi+r13]
    and r14, 0x00000000000000FF

    ; if null-termination, return.
    test r14, r14
    jz .parse_uint_get_number

    ; Ignore non-digit character.
    cmp r14, 0x30
    jb .parse_uint_loop_save_digit_inc_counter
    cmp r14, 0x39
    ja .parse_uint_loop_save_digit_inc_counter

    ; save the digit to the buffer.
    sub r14, 0x30
    mov [rsi+r15], r14
    inc r15

    .parse_uint_loop_save_digit_inc_counter:
    inc r13

    jmp .parse_uint_loop_save_digit_start
    

    .parse_uint_get_number:
    mov rdx, r15 ; get the length for return
    ; xor r15, r15 ; reset r15
    xor r14, r14 ; reset r14
    mov r13, 1 ; reset r13

    push rdx
    dec r15

    .parse_uint_loop_get_number:
    push rax
    mov rax, [rsi+r15] ; get a digit from right.
    and rax, 0x00000000000000FF
    mul r13 ; multiply...
    mov r14, rax
    pop rax

    add rax, r14 ; add to...

    test r15, r15
    jz .parse_uint_return

    dec r15

    push rax
    mov rax, r13
    mov r13, 10
    mul r13
    mov r13, rax
    pop rax

    jmp .parse_uint_loop_get_number


    .parse_uint_return:

    pop rdx

    ; clear stacks for buffer.
    pop r15
    pop r15
    pop r15

    pop r13
    pop r14
    pop r15
    pop rsi

    ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
    
    ; if the number is zero, print it immediately.
    cmp word[rdi], 0x0030
    jne .parse_int_non_zero_int
    mov rax, 0
    mov rdx, 1
    ret
    
    ; Get each character into number.
    .parse_int_non_zero_int:
    push rsi
    push r15
    push r14
    push r13
    push r12
    xor r15, r15  ; r15 will be used as counter
    xor r14, r14  ; r14 will be used as temporary store for digit
    xor r13, r13  ; r13 will be used as another counter
    xor r12, r12  ; r12 will store the sign.
                  ; 0 if it's positive, 1 if it's negative

    ; create buffer
    push 0
    push 0
    push 0
    mov rsi, rsp

    .parse_int_loop_save_digit_start:
    ; save each digit to the buffer.
    mov r14, [rdi+r13]
    and r14, 0x00000000000000FF

    ; if null-termination, return.
    test r14, r14
    jz .parse_int_get_number

    ; check if there's negative sign.
    cmp r14, 0x2d
    je .parse_int_loop_save_sign

    ; Ignore non-digit character.
    cmp r14, 0x30
    jb .parse_int_loop_save_digit_inc_counter
    cmp r14, 0x39
    ja .parse_int_loop_save_digit_inc_counter

    ; save the digit to the buffer.
    sub r14, 0x30
    mov [rsi+r15], r14
    inc r15

    .parse_int_loop_save_digit_inc_counter:
    inc r13

    jmp .parse_int_loop_save_digit_start
    
    .parse_int_loop_save_sign:
    mov r12, 1
    jmp .parse_int_loop_save_digit_inc_counter


    .parse_int_get_number:

    ; if there's no digit, return immediately.
    test r15, r15
    jnz .parse_int_loop_get_number_init

    pop r15
    pop r15
    pop r15

    pop r12
    pop r13
    pop r14
    pop r15
    pop rsi

    mov rax, 0
    mov rdx, 0
    ret

    .parse_int_loop_get_number_init:
    mov rdx, r15 ; get the length for return
    ; xor r15, r15 ; reset r15
    xor r14, r14 ; reset r14
    mov r13, 1 ; reset r13

    push rdx
    dec r15

    .parse_int_loop_get_number:
    push rax
    mov rax, [rsi+r15] ; get a digit from right.
    and rax, 0x00000000000000FF
    mul r13 ; multiply...
    mov r14, rax
    pop rax

    add rax, r14 ; add to...

    test r15, r15
    jz .parse_int_convert

    dec r15

    push rax
    mov rax, r13
    mov r13, 10
    mul r13
    mov r13, rax
    pop rax

    jmp .parse_int_loop_get_number


    .parse_int_convert:

    ; if it's negative, convert it to negative.
    test r12, r12
    je .parse_int_add_one_to_length

    not rax
    add rax, 1

    .parse_int_add_one_to_length:

    pop rdx

    ; if it's negative, add 1 to length for the sign
    test r12, r12
    je .parse_int_return

    add rdx, 1



    .parse_int_return:

    ; clear stacks for buffer.
    pop r15
    pop r15
    pop r15

    pop r12
    pop r13
    pop r14
    pop r15
    pop rsi


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
