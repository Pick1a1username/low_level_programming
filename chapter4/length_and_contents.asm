; This is my solution of question 66
%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

section .date

fname: db '/home/master/src/github/low_level_programming/chapter4/test.txt', 0  ; The file name.

section .bss
  stat resb 144

; https://stackoverflow.com/questions/27216616/get-file-size-with-stat-syscall
struc STAT
    .st_dev resq 1
    .st_ino resq 1
    .st_nlink resq 1
    .st_mode resd 1
    .st_uid resd 1
    .st_gid resd 1
    ;.pad0 resd 1
    .pad0 resb 4
    .st_rdev resq 1
    .st_size resq 1
    .st_blksize resq 1
    .st_blocks resq 1
    .st_atime resq 1
    .st_atime_nsec resq 1
    .st_mtime resq 1
    .st_mtime_nsec resq 1
    .st_ctime resq 1
    .st_ctime_nsec resq 1
endstruc

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

_start:
  ; call stat()
  mov rax, 4
  mov rdi, fname
  mov rsi, stat
  syscall
  xor rdi, rdi
  mov edi, dword [stat + STAT.st_size]
  call print_uint
  call print_newline

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



