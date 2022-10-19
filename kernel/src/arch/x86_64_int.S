.altmacro
.extern interrupt_handler
.global asm_interrupt_table, gdt_entries_load, asm_interrupt_handler_0, interrupt_enable, interrupt_disable
.extern current_interrupt_frame

.macro gen_interrupt_table index
    .quad asm_interrupt_handler_\index
.endm
.macro gen_interrupt_handler index
    asm_interrupt_handler_\index :
    //need to store the frame somewhere here
    push rax
    push rbx
    lea rax, [rip+current_interrupt_frame]
    
    mov rbx, [rsp+16]               //+16 because of the 2 push
    mov [rax], rbx                  //save RIP
    
    mov rbx, [rsp+24]
    mov [rax+8], rbx                //save CS
    
    mov rbx, [rsp+32]
    mov [rax+16], rbx               //save RFLAGS
    
    mov rbx, [rsp+40]
    mov [rax+24], rbx               //save RSP


    pop rbx
    pop rax
    //pop  rbx                         //store original rip
    //push rbx                         //somehow rebalance the stack do not remove for now
    pushad
    cli                              //disable interrupts
    cld                              //needed for sysV abi
    push rdi
    mov  rdi, \index                 //interrupt number
    mov  rsi, rsp                    //stack pointer to get the frame
    call interrupt_handler           //calls the C code
    pop  rdi
    push rax
    mov  al, 0x20                   //ack PIC int
    out  0x20, al
    
    lea  rax, [rip+current_interrupt_frame] //clear current_interrupt_frame
    mov qword ptr [rax   ], 0       
    mov qword ptr [rax+8 ], 0
    mov qword ptr [rax+16], 0
    mov qword ptr [rax+24], 0               //

    pop  rax
    sti                              //enable interrupts
    popad
    iretq
.endm
.macro gen_interrupt_handler_loop
    i=0
    .rept 256
        gen_interrupt_handler %i
    i=i+1
    .endr
.endm
.macro gen_interrupt_table_loop
    i=0
    .rept 256
        gen_interrupt_table %i
    i=i+1
    .endr
.endm

.macro pushad
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
.endm
.macro popad
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdx
    pop rcx
    pop rbx
    pop rax
.endm
.section .text

gen_interrupt_handler_loop
asm_interrupt_table:
gen_interrupt_table_loop

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

interrupt_enable:
    sti
    ret
interrupt_disable:
    cli
    ret
