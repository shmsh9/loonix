.altmacro
.extern interrupt_handler
.global asm_interrupt_table, gdt_entries_load, asm_interrupt_handler_0, interrupt_enable, interrupt_disable, task_interrupt_enable
.extern current_interrupt_frame, task_current
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
#define cpu_registers_xmm0 136
#define cpu_registers_flags 152
.section .data

.macro gen_interrupt_table index
    .quad asm_interrupt_handler_\index
.endm
.macro gen_interrupt_handler index
    asm_interrupt_handler_\index :
    cli                              //disable interrupts
    
    //need to store the frame somewhere here
    .task_save_context_begin_\index :
    push rax
    push rbx
    mov  rax, \index
    cmp  rax, 0x20                      //only save context for timer tasks
    jne   .task_save_context_end_\index
    lea  rax, [rip+task_current]
    cmp  qword ptr [rax], 0
    je   .task_save_context_end_\index
    
    //// Save current context
    mov  rax, [rax]               //rax == *task_current
    add  rax, 16                  //
    mov  rax, [rax]               //rax == *task_current->context

    mov rbx, qword ptr [rsp+8]         //because of the 2 push now contains old rax
    //crashes here after sometime maybe task_current gets corrupted
    mov [rax+cpu_registers_rax], rbx
    mov rbx, qword ptr [rsp]            //because of the 2 push now contains old rbx
    mov [rax+cpu_registers_rbx], rbx

    mov [rax+cpu_registers_rcx],rcx
    mov [rax+cpu_registers_rdx],rdx
    mov [rax+cpu_registers_rsi],rsi
    mov [rax+cpu_registers_rdi],rdi
    mov rbx, [rsp+40]               // +40 == 2 push rax rbx && interrupt frame 32Bytes 
    mov [rax+cpu_registers_rsp],rbx //
    mov [rax+cpu_registers_rbp],rbp
    mov [rax+cpu_registers_r8 ],r8
    mov [rax+cpu_registers_r9 ],r9
    mov [rax+cpu_registers_r10],r10
    mov [rax+cpu_registers_r11],r11
    mov [rax+cpu_registers_r12],r12
    mov [rax+cpu_registers_r13],r13
    mov [rax+cpu_registers_r14],r14
    mov [rax+cpu_registers_r15],r15
    mov rbx, [rsp+16]                   //old rip stored on the stack by the interrupt
    mov [rax+cpu_registers_rip], rbx
    movdqu [rax+cpu_registers_xmm0], xmm0 //the only one I use
    mov rbx, [rsp+32]                  //old rflags stored on the stack by the interrupt
    mov [rax+cpu_registers_flags], rbx
    /////////////////////////////////////
    .task_save_context_end_\index :
    //pop rbx
    //pop rax
    //saving current interrupt frame
    //push rax
    //push rbx
    lea rax, [rip+current_interrupt_frame]
    
    mov rbx, [rsp+16]               //+16 because of the 2 push
    mov [rax], rbx                  //save RIP
    
    mov rbx, [rsp+24]
    mov [rax+8], rbx                //save CS
    
    mov rbx, [rsp+32]
    mov [rax+16], rbx               //save RFLAGS
    
    mov rbx, [rsp+40]
    mov [rax+24], rbx               //save RSP
    
    //pop rbx
    //pop rax
    ///////////////////////////////////////
    pushad
    cld                              //needed for sysV abi
    push rdi
    mov  rdi, \index                 //interrupt number
    mov  rsi, rsp                    //stack pointer to get the frame
    call interrupt_handler           //calls the C code
    pop  rdi
    //ack PIC//////////////////
    //push rax
    mov  al, 0x20
    out  0x20, al
    //////clear current_interrupt_frame
    lea  rax, [rip+current_interrupt_frame] 
    mov qword ptr [rax   ], 0       
    mov qword ptr [rax+8 ], 0
    mov qword ptr [rax+16], 0
    mov qword ptr [rax+24], 0
    ////////////////////////////
    popad
    pop  rbx
    pop  rax
    //sti                              //enable interrupts
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
    sub    rsp, 16
    movdqu [rsp], xmm0
.endm
.macro popad
    movdqu xmm0, [rsp]
    add rsp, 16
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
//void task_interrupt_enable()
task_interrupt_enable:
    push rax
    mov  al, 0x20                   //ack PIC int
    out  0x20, al
    sti
    pop rax
    ret
