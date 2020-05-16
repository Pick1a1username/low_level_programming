%define O_RDONLY 0
%define PROT_READ 0x1
%define MAP_PRIVATE 0x2

%assign limit 29

; this directive is necessary?
section .data

is_prime: dd 0x1  ; 32bits
fname: db 'input.txt', 0 ; the file name

; this directive is necessary?
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


check_prime:
  xor rax, rax
  push rdi
  push rsi
  mov eax, dword[is_prime]

.check_prime_loop:
  test rdi, rdi
  jz .check_prime_return

  shl eax, 1
  dec rdi
  jmp .check_prime_loop

.check_prime_return:
  and rax, 0x0000000080000000
  shr rax, 31

  pop rsi
  pop rdi
  ret

_start:

  %assign n 3
  %rep limit
    shl dword [is_prime], 1
    %assign current 1
    %assign i 1
      %rep n/2
        %assign i i+1
        %if n % i = 0
          %assign current 0
          %exitrep
        %endif
      %endrep
    add dword [is_prime], current
    %assign n n+1
  %endrep

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

  push rax
  mov rdi, rax
  call print_string
  pop rax

  push rdx
  mov rdi, rax
  call parse_uint
  pop rdx

  mov rdi, rax
  call check_prime

  mov rdi, rax
  call print_uint

  call print_newline




  mov rax, 60
  xor rdi, rdi
  syscall


