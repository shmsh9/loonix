.extern exception_handler
.global asm_interrupt_table, gdt_entries_load, asm_interrupt_handler_0

.section .text
asm_interrupt_table:
    .quad asm_interrupt_handler_0
    .quad asm_interrupt_handler_1
    .quad asm_interrupt_handler_2
    .quad asm_interrupt_handler_3
asm_interrupt_handler_0:
    pop  rbx //store original rip
    push rbx //somehow rebalance the stack do not remove for now
    cli  //disable interrupts
    cld  //needed for sysV abi
    mov  rdi, 0 //interrupt number
    call exception_handler
    sti //enable interrupts
    iretq
asm_interrupt_handler_1:
    pop  rbx //store original rip
    push rbx //somehow rebalance the stack do not remove for now
    cli  //disable interrupts
    cld  //needed for sysV abi
    mov  rdi, 1 //interrupt number
    call exception_handler
    sti //enable interrupts
    iretq
asm_interrupt_handler_2:
    pop  rbx //store original rip
    push rbx //somehow rebalance the stack do not remove for now
    cli  //disable interrupts
    cld  //needed for sysV abi
    mov  rdi, 2//interrupt number
    call exception_handler
    sti //enable interrupts
    iretq
asm_interrupt_handler_3:
    pop  rbx //store original rip
    push rbx //somehow rebalance the stack do not remove for now
    cli  //disable interrupts
    cld  //needed for sysV abi
    mov  rdi, 3 //interrupt number
    call exception_handler
    sti //enable interrupts
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
