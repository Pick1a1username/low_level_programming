; This is my solution of question 67
%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

section .date

fname: db 'input.txt', 0  ; The file name.

section .text

global _start

print_string:
  push rdi
  call string_length
  pop rsi
  mov rdx, rax
  mov rax, 1
  mov rdi, 1
  syscall
  ret

string_length:
  xor rax, rax
.loop:
  cmp byte [rdi+rax], 0
  je .end
  inc rax
  jmp .loop
.end:
  ret

print_uint:
    mov rax, rdi
    mov rdi, rsp
    push 0
    sub rsp, 16
    
    dec rdi
    mov r8, 10

.loop:
    xor rdx, rdx
    div r8
    or  dl, 0x30
    dec rdi 
    mov [rdi], dl
    test rax, rax
    jnz .loop 
   
    call print_string
    
    add rsp, 24
    ret

print_char:
    push rdi
    mov rdi, rsp
    call print_string 
    pop rdi
    ret

print_newline:
    mov rdi, 10
    jmp print_char

; returns rax: number, rdx : length
parse_uint:
    mov r8, 10
    xor rax, rax
    xor rcx, rcx
.loop:
    movzx r9, byte [rdi + rcx] 
    cmp r9b, '0'
    jb .end
    cmp r9b, '9'
    ja .end
    xor rdx, rdx 
    mul r8
    and r9b, 0x0f
    add rax, r9
    inc rcx 
    jmp .loop 
    .end:
    mov rdx, rcx
    ret

; The code above is from the following link:
; https://github.com/Apress/low-level-programming/blob/master/assignments/2_dictionary/teacher/lib.asm


; arguments
; * rdi: the number
; returns
; * rax: 
factorial:
    push rdi
    mov rax, rdi ; set the one of operands

.factorial_loop:
    dec rdi
    test rdi, rdi
    je .factorial_return

    mul rdi ; multiply...
    jmp .factorial_loop

.factorial_return:
    pop rdi
    ret


_start:
  ; call open()
  mov rax, 2 
  mov rdi, fname
  mov rsi, O_RDONLY   ; Opoen file read only
  mov rdx, 0          ; We are not creating a file
                      ; so this argument has no meaning.
  syscall

  ; call mmap()
  mov r8, rax           ; rax holds opened file descriptor
                        ; it is the fourth argument of mmap
  mov rax, 9            ; mmap number
  mov rdi, 0            ; operating system will choose mapping destination.
  mov rsi, 4096         ; page size
  mov rdx, PROT_READ    ; new memory region will be marked read only.
  mov r10, MAP_PRIVATE  ; pages will not be shared.
  mov r9, 0             ; offset inside test.txt
  syscall               ; now rax will point to mapped location
  
  ; print the string
  push rax

  mov rdi, rax
  call print_string

  pop rax  ; get the string back

  ; parse an integer from the string
  mov rdi, rax
  call parse_uint

  ; the factorial of the integer
  push rax

  mov rdi, rax
  call factorial
  
  mov rdi, rax
  call print_uint
  call print_newline

  pop rax  ; get the integer back

  ; check whether it's prime or not


  ; sum of all number's digits

  ; x-th Fibonacci number

  ; check if x is a Fibonacci number



  ; exit
  mov rax, 60
  xor rdi, rdi
  syscall

