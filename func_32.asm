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

.section .text
.global add_32

add_32:
    movl %edi, %eax
    addl %esi, %eax
    ret