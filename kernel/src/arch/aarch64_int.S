//https://krinkinmu.github.io/2021/01/10/aarch64-interrupt-handling.html
.macro interruptdisable
    msr daifclr, #2
.endm
.macro interruptenable
    msr daifset, #2
.endm
.macro pushreg
    stp x0,  x1,  [sp, #-16   ]!
    stp x2,  x3,  [sp, #-16*2 ]!
    stp x4,  x5,  [sp, #-16*3 ]!
    stp x6,  x7,  [sp, #-16*4 ]!
    stp x10, x9,  [sp, #-16*5 ]!
    stp x12, x13, [sp, #-16*6 ]!
    stp x14, x15, [sp, #-16*7 ]!
    stp x20, x19, [sp, #-16*8 ]!
    stp x22, x21, [sp, #-16*9 ]!
    stp x24, x23, [sp, #-16*10]!
    stp x26, x25, [sp, #-16*11]!
    stp x28, x27, [sp, #-16*12]!

.endm
.macro popreg
    ldp x28, x27, [sp], 16
    ldp x26, x25, [sp], 16*2
    ldp x24, x23, [sp], 16*3
    ldp x22, x21, [sp], 16*4
    ldp x20, x19, [sp], 16*5
    ldp x14, x15, [sp], 16*6
    ldp x12, x13, [sp], 16*7
    ldp x10, x9,  [sp], 16*8
    ldp x6,  x7,  [sp], 16*9
    ldp x4,  x5,  [sp], 16*10
    ldp x2,  x3,  [sp], 16*11
    ldp x0,  x1,  [sp], 16*12
.endm

.text
.global interrupt_vector_table, init_interrupt_vector_table, interrupt_enable, interrupt_disable
.extern interrupt_handler
.balign 2048
interrupt_vector_table:
interrupt_vector_table_el0:
    b .
.balign 0x80
    b .
.balign 0x80
    b .
.balign 0x80
    b .
interrupt_vector_table_el1:
    pushreg
    interruptdisable
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #0
    bl interrupt_handler
    popreg
    interruptenable
    eret
.balign 0x80
    el1_irq:
    pushreg
    interruptdisable
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #0
    bl interrupt_handler
    popreg
    interruptenable
    eret
.balign 0x80
    .el1_fiq:
    pushreg
    interruptdisable
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #2
    bl interrupt_handler
    popreg
    interruptenable
    eret
.balign 0x80
    .el1_error:
    pushreg
    interruptdisable
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #3
    bl interrupt_handler
    popreg
    interruptenable
    eret
interrupt_vector_table_el2:
    b .
.balign 0x80
    b .
.balign 0x80
    b .
.balign 0x80
    b .
interrupt_vector_table_el3:
    b .
.balign 0x80
    b .
.balign 0x80
    b .
.balign 0x80
    b .
interrupt_vector_table_el4:
    b .
.balign 0x80
    b .
.balign 0x80
    b .
.balign 0x80
    b .

//void init_interrupt_vector_table()
init_interrupt_vector_table:
    adr  x0, interrupt_vector_table
    msr   VBAR_EL1, x0
    ret

interrupt_disable:
    interruptdisable
    ret
interrupt_enable:
    interruptenable
    ret

//void process_switch_context(process *x0)