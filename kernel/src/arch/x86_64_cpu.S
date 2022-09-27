.global cpu_registers_save, cpu_registers_load, cpu_get_tick, __memcpy_64b, __memset_64b, __memset_128b, __memcpy_128b, exception_handler
.section .text
//void __memset_64b(void *ptr = %rdi, uint64_t b = %rsi, uint64_t sz = %rdx)
__memset_64b:
    __memset_64b_loop:
    mov  [rdi],rsi
    add  rdi, 8
    sub  rdx, 8
    jnz  __memset_64b_loop
    ret
//void __memset_128b(void *ptr = %rdi, uint64_t b[2] = %rsi, uint64_t sz = %rdx)
__memset_128b:
    movdqu xmm0, [rsi]
    __memset_128b_loop:
    movdqu [rdi], xmm0
    add    rdi, 16
    sub    rdx, 16
    jnz   __memset_128b_loop
    ret
//void __memcpy_64b(void *dst = %rdi, void *src = %rsi, uint64_t sz = %rdx)
__memcpy_64b:
    mov rcx, rdx
    shr rcx, 3
    rep movsq
    ret
//void __memcpy_128b(void *dst = %rdi, void *src = %rsi, uint64_t sz = %rdx)
__memcpy_128b:
    __memcpy_128b_loop:
    movdqu  xmm0, [rsi]
    movdqu  [rdi], xmm0
    add     rsi, 16
    add     rdi, 16
    sub     rdx, 16
    jnz     __memcpy_128b_loop
    ret

//void cpu_registers_save(cpu_registers *regs);
cpu_registers_save:
    mov [rdi], rax
    mov [rdi+8], rbx
    mov [rdi+16],rcx
    mov [rdi+24],rdx
    mov [rdi+32],rsi
    mov [rdi+40],rdi
    mov [rdi+48],rbp
    mov [rdi+56],rsp
    mov [rdi+64],r8
    mov [rdi+72],r9
    mov [rdi+80],r10
    mov [rdi+88],r11
    mov [rdi+96],r12
    mov [rdi+104],r13
    mov [rdi+112],r14
    mov [rdi+128],r15
    lea rax, [rip]
    mov [rdi+136],rax
    ret
//void cpu_registers_load(cpu_registers *regs);
//Need to modify RIP
cpu_registers_load:
    mov rax,[rdi]
    mov rbx,[rdi+8]
    mov rcx,[rdi+16]
    mov rdx,[rdi+24]
    mov rsi,[rdi+32]
    mov rbp,[rdi+48]
    mov rsp,[rdi+56]
    mov r8,[rdi+64]
    mov r9,[rdi+72]
    mov r10,[rdi+80]
    mov r11,[rdi+88]
    mov r12,[rdi+96]
    mov r13,[rdi+104]
    mov r14,[rdi+112]
    mov r15,[rdi+128]
    mov rdi,[rdi+40]
    ret
//uint64_t cpu_get_tick()
cpu_get_tick:
    xor     rax, rax
    xor     rdx, rdx
    rdtsc
    shl     rdx, 32
    or      rax, rdx
    ret
