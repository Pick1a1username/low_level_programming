%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

section .date

fname: db 'test.txt', 0  ; The file name.

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
  
  mov rdi, rax
  call print_string

  mov rax, 60
  xor rdi, rdi
  syscall



