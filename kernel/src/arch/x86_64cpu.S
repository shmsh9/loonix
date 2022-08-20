.global cpu_registers_save, cpu_registers_load, cpu_get_tick, gdt_entries_load, __memcpy_64b, __memset_64b

//void __memset_64b(void *ptr = %rdi, uint64_t b = %rsi, uint64_t sz = %rdx)
__memset_64b:
    __memset_64b_loop:
    mov  %rax, (%rdi)
    add  $8 ,%rdi
    sub  $8, %rdx
    jnz  __memset_64b_loop
    ret
//void __memcpy_64b(void *dst = %rdi, void *src = %rsi, uint64_t sz = %rdx)
__memcpy_64b:
    mov %rdx, %rcx
    shr $3, %rcx
    rep movsq
    ret
//void gdt_entries_load(gdt_ptr *ptr)
gdt_entries_load:
    /*
    lgdt (%rdi)
    movq 2(%rdi), %rdi   // rdi = ptr->offset
    movq 8*3(%rdi), %rdi // rdi == ptr->offset[3]
    push %rdi
    lea  gdt_reload_segments, %rax
    push %rax
    lretq
gdt_reload_segments:
    mov  $0x10, %ax
    mov  %ax, %ds
    mov  %ax, %es
    mov  %ax, %fs
    mov  %ax, %gs
    mov  %ax, %ss
    */
    ret
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
