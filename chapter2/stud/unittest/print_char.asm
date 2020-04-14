section .text
; %include "lib.inc"
print_char:
    xor rax, rax

    ; most of codes are copied from hello.asm
    push rdi
    push r11
    mov  r11, rdi
    mov  rsi, [r11]  ;argument #2 in rsi: where does the string start
    mov  rdi, 1        ;argument #1 in rdi: where to write (descriptor)?
    mov  rax, 1        ;system call number should be stored in rax
    mov  rdx, 8       ;argument #3 in rdx: how many bytes to write
    push rcx           ; syscall will break rcx
    syscall            ;this instruction invokes a system call
    pop rcx
    pop r11
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

mov rdi, 'a'
call print_char


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

