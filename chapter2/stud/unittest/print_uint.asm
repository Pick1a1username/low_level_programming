section .text
; %include "lib.inc"

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
  push rsi
  ; Create a buffer
  mov r11, 0x0000000000000000
  push r11
  mov rsi, rsp
  push rax
  push rbx
  push rdi
  mov rax, rdi
  mov bl, 10 ; divider
  ; mov rbx, 10
  ; mov r12, 0
  .loop_get_decimal_digits:
  mov ah, 0 ; reset remainder
  ; https://www.tutorialspoint.com/assembly_programming/assembly_arithmetic_instructions.htm
  div bl
  ; div rbx <- Floating point exception (core dumped)
  ; quotient is stored in al
  ; remainder is stored in ah
  mov [rsi], ah ; copy the digit to the buffer
  inc rsi
  ; cmp 0, al   <- This doesn't work!
  cmp al, 0
  je .print_uint_print_digits
  jmp .loop_get_decimal_digits
  .print_uint_print_digits:
  pop rdi
  pop rbx
  pop rax
  pop r11
  pop rsi

  push rdi
  push rbx
  push rcx
  push r12

  ; mov rbx, [r11] <- This doesn't work..(segmentation fault)
  ; lea rbx, [r11]
  

  mov rcx, 64   ; for shifting the register storing digits
  mov r12, 0    ; 0 if digit is not started?
  
  jmp .loop_print_digits
  .pre_loop_print_digits:
  pop rdx
  pop rdi
  pop r11
  pop rax

  .loop_print_digits:
  push rax
  push r11
  push rdi
  push rdx

  sub rcx, 8
  sar r11, cl
  and r11, 0xff

  ; if it's digit, start to print
  cmp r12, 0
  jne .start_print

  ; if...
  cmp r11, 0 
  je .pre_loop_print_digits
  mov r12, 1

  .start_print:
  mov rsi, 0x30
  add rsi, r11

  push rsi
  mov rsi, rsp  ; argument #2 in rsi: where does the string start
  mov rdx, 1  ; argument #3 in rdx: how many bytes to write
  mov rax, 1  ; system call number should be stored in rax
  mov rdi, 1  ; argument #1 in rdi: where to write (descriptor)?
  push rcx
  syscall
  pop rcx
  pop rsi
  
  pop rdx
  pop rdi
  pop r11
  pop rax

  test rcx, rcx
  jnz .loop_print_digits
  
  call print_newline

  pop r12
  pop rcx
  pop rbx
  pop rdi

  ret

global _start 
_start:
; before_call
mov rdi, -1
mov rsi, -1
mov rax, -1
mov rcx, -1
mov rdx, -1
mov r8, -1
mov r9, -1
mov r10, -1
mov r11, -1
push rbx
push rbp
push r12
push r13
push r14
push r15
; before_call(end)

; mov rdi, 65536 <- infinite loop
; mov rdi, 128 ; <- OK
; mov rdi, 192 ; <- OK
mov rdi, 255 ; <- OK
; mov rdi, 0 <- infinite loop
; mov rdi, 1024 <- infinite loop
; mov rdi, 512  <- infinite loop
; mov rdi, 256  <- infinite loop
call print_uint

; after_call
cmp r15, [rsp]
jne .convention_error
pop r15
cmp r14, [rsp]
jne .convention_error
pop r14
cmp r13, [rsp]
jne .convention_error
pop r13
cmp r12, [rsp]
jne .convention_error
pop r12
cmp rbp, [rsp]
jne .convention_error
pop rbp
cmp rbx, [rsp]
jne .convention_error
pop rbx

jmp continue

.convention_error:
    mov rax, 1
    mov rdi, 2
    mov rsi, err_calling_convention
    mov rdx, err_calling_convention.end - err_calling_convention
    syscall
    mov rax, 60
    mov rdi, -41
    syscall

section .data
err_calling_convention: db "You did not respect the calling convention! Check that you handled caller-saved and callee-saved registers correctly", 10

.end:
section .text
continue:
; after_call(end)

mov rax, 60
xor rdi, rdi
syscall

