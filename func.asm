section .text
global add_64

add_64:
    ; Linux x86_64 调用约定:
    ; rdi = 第一个参数 (a)
    ; rsi = 第二个参数 (b)
    mov rax, rdi    ; 把 a 挪到 rax
    add rax, rsi    ; rax = rax + rsi
    ret             ; 返回值永远放在 rax

;int add_32(int a, int b) {
;    int result;
;    // GCC 风格的内联汇编
;    asm (
;        "movl %%ebx, %%eax;"
;        "addl %%ecx, %%eax;"
;        : "=a" (result)         // 输出：将 eax 的值赋给 result
;        : "b" (a), "c" (b)      // 输入：将 a 放入 ebx，b 放入 ecx
;    );
;    return result;
;}
;
;.section .text
;.global add_32
;
;add_32:
;    movl %edi, %eax
;    addl %esi, %eax
;    ret