.global cpu_registers_save, cpu_registers_load, cpu_get_tick
//void cpu_registers_save(cpu_registers *regs);
cpu_registers_save:
    movq %rax, (%rdi)   
    movq %rbx, 8  (%rdi)
    movq %rcx, 16 (%rdi)
    movq %rdx, 24 (%rdi)
    movq %rsi, 32 (%rdi)
    movq %rdi, 40 (%rdi)
    movq %rbp, 48 (%rdi)
    movq %rsp, 56 (%rdi)
    movq %r8,  64 (%rdi)
    movq %r9,  72 (%rdi)
    movq %r10, 80 (%rdi)
    movq %r11, 88 (%rdi)
    movq %r12, 96 (%rdi)
    movq %r13, 104(%rdi)
    movq %r14, 112(%rdi)
    movq %r15, 128(%rdi)
    ret
//void cpu_registers_load(cpu_registers *regs);
cpu_registers_load:
    movq %rax,    (%rdi)   
    movq %rbx,  8 (%rdi)
    movq %rcx, 16 (%rdi)
    movq %rdx, 24 (%rdi)
    movq %rsi, 32 (%rdi)
    movq %rdi, 40 (%rdi)
    movq %rbp, 48 (%rdi)
    movq %rsp, 56 (%rdi)
    movq %r8,  64 (%rdi)
    movq %r9,  72 (%rdi)
    movq %r10, 80 (%rdi)
    movq %r11, 88 (%rdi)
    movq %r12, 96 (%rdi)
    movq %r13, 104(%rdi)
    movq %r14, 112(%rdi)
    movq %r15, 128(%rdi)
    ret
//uint64_t cpu_get_tick()
cpu_get_tick:
    xor     %rax, %rax
    xor     %rdx, %rdx
    rdtsc
    shl     $32,  %rdx
    or      %rdx, %rax
    ret