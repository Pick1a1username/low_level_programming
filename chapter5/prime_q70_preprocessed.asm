%line 4+1 prime_q70.asm



is_prime: dd 0x4
fname: db 'input.txt', 0


[global _start]

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
 or dl, 0x30
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
 mov eax, dword[is_prime]

.check_prime_loop:
 test rdi, rdi
 jz .check_prime_return

 shr rax, 1
 dec rdi
 jmp .check_prime_loop

.check_prime_return:
 and rax, 0x0000000000000001
 pop rdi
 ret

_start:

%line 124+1 prime_q70.asm
 add dword [is_prime], 1
%line 124+0 prime_q70.asm
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 0
 shl dword [is_prime], 1
 add dword [is_prime], 1
 shl dword [is_prime], 1
%line 125+1 prime_q70.asm


 mov rax, 2
 mov rdi, fname
 mov rsi, 0
 mov rdx, 0

 syscall


 mov r8, rax

 mov rax, 9
 mov rdi, 0
 mov rsi, 4096
 mov rdx, 0x1
 mov r10, 0x2
 mov r9, 0
 syscall

 push rax
 mov rdi, rax
 call print_string
 pop rax

















 mov rax, 60
 xor rdi, rdi
 syscall


