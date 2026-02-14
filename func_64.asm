section .text
global add_64

add_64:
    ; Linux x86_64 调用约定:
    ; rdi = 第一个参数 (a)
    ; rsi = 第二个参数 (b)
    mov rax, rdi    ; 把 a 挪到 rax
    add rax, rsi    ; rax = rax + rsi
    ret             ; 返回值永远放在 rax