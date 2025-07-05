section .data
fmt_int: db "%d", 10, 0
str0: db "Pi", 0
str1: db ":", 0

section .bss
p:    resq    1

section .text
extern malloc
extern free
extern printf
global main
main:
    mov    rdi, 16
    call   malloc
    mov    qword [p], rax
    lea    rax, [p]
    mov    rax, [rax]
    lea    rax, [rax + 0]
    mov    rbx, rax
    mov    rax, str0
    mov    [rbx], rax
    lea    rax, [p]
    mov    rax, [rax]
    lea    rax, [rax + 8]
    mov    rbx, rax
    mov    rax, 0x40091EB851EB851F
    mov    [rbx], rax
    mov    rdi, fmt_int
    lea    rax, [p]
    mov    rax, [rax]
    lea    rax, [rax + 0]
    mov    rax, [rax]
    mov    rsi, rax
    xor    rax, rax
    call   printf
    mov    rdi, [p]
    call   free
    ret
