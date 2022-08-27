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
    lgdt    [rdi]
    push    0x10  //kernel segment
    lea     rax, [.gdt_reload_segments]
    push    rax
    retfq
    //somehow switch to 32 bits ??
.gdt_reload_segments:
    //0x20 == data segment
    mov  ax, 0x20
    mov  ds, ax
    mov  es, ax
    mov  fs, ax
    mov  gs, ax
    mov  ss, ax
    ret

