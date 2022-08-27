.extern exception_handler
.global asm_interrupt_handler, gdt_entries_load

.section .text

asm_interrupt_handler:
    push rax
    push rcx
    push rdx
    push rbx
    push rsp
    push rbp
    push rsi
    push rdi
    call exception_handler
    pop  rdi
    pop  rsi
    pop  rbp
    pop  rsp
    pop  rbx
    pop  rdx
    pop  rcx
    pop  rax
    iretq

//void gdt_entries_load(gdt_ptr *rdi)
gdt_entries_load:
    lgdt [rdi]
    sub  rsp, 8
    mov  qword ptr [rsp], 0x8
    lea  rax, [gdt_entries_end+rip]
    push rax
    retfq
    //jump in 32bits mod should check flags
    gdt_entries_end:
    mov  ax, 0x10 //kernel data section
    mov  ss, ax
    mov  ds, ax
    mov  es, ax
    ret
