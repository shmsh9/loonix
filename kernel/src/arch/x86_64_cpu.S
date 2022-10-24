.global cpu_registers_save, task_cpu_registers_load, cpu_registers_load2, cpu_get_tick, __memcpy_64b, __memset_64b, __memset_128b, __memcpy_128b, exception_handler
.section .text
.extern current_interrupt_frame

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
#define cpu_registers_rax 0
#define cpu_registers_rbx 8
#define cpu_registers_rcx 16
#define cpu_registers_rdx 24
#define cpu_registers_rsi 32
#define cpu_registers_rdi 40
#define cpu_registers_rbp 48
#define cpu_registers_rsp 56
#define cpu_registers_r8  64
#define cpu_registers_r9  72
#define cpu_registers_r10 80
#define cpu_registers_r11 88
#define cpu_registers_r12 96
#define cpu_registers_r13 104
#define cpu_registers_r14 112
#define cpu_registers_r15 120
#define cpu_registers_rip 128
//void cpu_registers_save(cpu_registers *regs);
cpu_registers_save:
    mov [rdi+cpu_registers_rax], rax
    mov [rdi+cpu_registers_rbx], rbx
    mov [rdi+cpu_registers_rcx],rcx
    mov [rdi+cpu_registers_rdx],rdx
    mov [rdi+cpu_registers_rsi],rsi
    mov [rdi+cpu_registers_rdi],rdi
    mov [rdi+cpu_registers_rbp],rbp
    mov [rdi+cpu_registers_rsp],rsp
    mov [rdi+cpu_registers_r8 ],r8
    mov [rdi+cpu_registers_r9 ],r9
    mov [rdi+cpu_registers_r10],r10
    mov [rdi+cpu_registers_r11],r11
    mov [rdi+cpu_registers_r12],r12
    mov [rdi+cpu_registers_r13],r13
    mov [rdi+cpu_registers_r14],r14
    mov [rdi+cpu_registers_r15],r15
    lea rax, [rip]
    mov [rdi+cpu_registers_rip],rax
    ret
//void task_cpu_registers_load(cpu_registers *regs);
task_cpu_registers_load:
    mov rbp,[rdi+cpu_registers_rbp] 
    mov rsp,[rdi+cpu_registers_rsp] //so that our push affects the newly allocated stack
    mov rax,[rdi+cpu_registers_rip] 
    push rax                        //this push for the rip return address
    mov rax,[rdi+cpu_registers_rax]
    mov rbx,[rdi+cpu_registers_rbx]
    mov rcx,[rdi+cpu_registers_rcx]
    mov rdx,[rdi+cpu_registers_rdx]
    mov rsi,[rdi+cpu_registers_rsi]
    mov r8, [rdi+cpu_registers_r8 ]
    mov r9, [rdi+cpu_registers_r9 ]
    mov r10,[rdi+cpu_registers_r10]
    mov r11,[rdi+cpu_registers_r11]
    mov r12,[rdi+cpu_registers_r12]
    mov r13,[rdi+cpu_registers_r13]
    mov r14,[rdi+cpu_registers_r14]
    mov r15,[rdi+cpu_registers_r15]
    mov rdi,[rdi+cpu_registers_rdi]
    //ack PIC && reenable interrupts//
    push rax
    mov  al, 0x20                   //ack PIC int
    out  0x20, al
    sti
    pop rax
    ///////////////////////////////////
    ret

//uint64_t cpu_get_tick()
cpu_get_tick:
    xor     rax, rax
    xor     rdx, rdx
    rdtsc
    shl     rdx, 32
    or      rax, rdx
    ret
