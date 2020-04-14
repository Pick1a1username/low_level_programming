section .data
arg1: db 'ashdb asdhabs dahb', 0
arg2: times 19 db  66

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

; rdi: string
; rsi: buffer
; rdx: buffer's length
string_copy:
    xor rax, rax

    ; Check the string is smaller than the buffer.
    push rdi
    push rsi
    push rdx
    call string_length
    pop rdi
    pop rsi
    pop rdx


    cmp rax, rdx    ; string's length vs buffer's length
    jbe .copy_str

    xor rax, rax      ; If the string is bigger than the buffer, return zero.
    ret
.copy_str:
    ; If the buffer is enoughly big, copy the string to the buffer.
    ; https://www.cs.virginia.edu/~evans/cs216/guides/x86.html
    mov [rsi], rdi
    lea rax, [rsi]

    ret

    ; rdi = source
    ; rsi = dest
    ; rdx = dest length


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

push rdi
push rsi
push rdx

mov rdi, arg1
mov rsi, arg2
mov rdx, 19
;call string_copy

pop rdi
pop rsi
pop rdx

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

mov rdi, arg2 
call print_string
mov rax, 60
xor rdi, rdi
syscall
