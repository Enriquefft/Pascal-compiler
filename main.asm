section .data
fmt_int: db "%d", 10, 0

section .bss
a:    resq    1

section .text
extern printf
global main

main:
    mov    rdi, fmt_int
    mov    rax, [a + 0]
    mov    rsi, rax
    xor    rax, rax
    call   printf
    ret
