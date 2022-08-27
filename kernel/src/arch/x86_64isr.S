.extern exception_handler, idt_stub_table
.global asm_interrupt_handler, gdt_entries_load

.section .text
asm_interrupt_handler:
    pop  rax //store return address
    sub  rsp, 8
    mov  qword ptr [rsp], 0x8 //return to kernel mode
    push rax
    cli  //disable interrupts
    call exception_handler
    sti //enable interrupts
    retfq

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
