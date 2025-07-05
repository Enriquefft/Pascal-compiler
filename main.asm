section .data
fmt_int: db "%d", 10, 0
str0: db "Pi", 0
str1: db ":", 0

section .bss
name:    resq    1
value:    resq    1
p:    resq    1

section .text
extern malloc
extern free
extern printf
global main
main:
    mov    rdi, 8
    call   malloc
    mov    qword [p], rax
    mov    rax, [p]
    mov    qword [rax], str0
    mov    rax, [p]
    mov    qword [rax], 0x40091EB851EB851F
    mov    rdi, fmt_int
    mov    rax, [p]
    mov    rsi, rax
    xor    rax, rax
    call   printf
    mov    rdi, [p]
    call   free
    ret
