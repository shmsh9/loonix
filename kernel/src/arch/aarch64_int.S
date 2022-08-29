//https://krinkinmu.github.io/2021/01/10/aarch64-interrupt-handling.html
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
    bl  interrupt_disable
    sub sp, sp, #16
    stp x0, x1, [sp]
    sub sp, sp, #8
    str x2, [sp]
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #0
    bl interrupt_handler
    ldr x2, [sp]
    add sp, sp, #8
    ldp x0, x1, [sp]
    add sp, sp, #16
    bl interrupt_enable
    eret
.balign 0x80
    el1_irq:
    bl  interrupt_disable
    sub sp, sp, #16
    stp x0, x1, [sp]
    sub sp, sp, #8
    str x2, [sp]
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #1
    bl interrupt_handler
    ldr x2, [sp]
    add sp, sp, #8
    ldp x0, x1, [sp]
    add sp, sp, #16
    bl interrupt_enable
    eret
.balign 0x80
    .el1_fiq:
    bl  interrupt_disable
    sub sp, sp, #16
    stp x0, x1, [sp]
    sub sp, sp, #8
    str x2, [sp]
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #2
    bl interrupt_handler
    ldr x2, [sp]
    add sp, sp, #8
    ldp x0, x1, [sp]
    add sp, sp, #16
    bl interrupt_enable
    eret
.balign 0x80
    .el1_error:
    bl  interrupt_disable
    sub sp, sp, #16
    stp x0, x1, [sp]
    sub sp, sp, #8
    str x2, [sp]
    mrs x0, FAR_EL1
    mrs x1, ESR_EL1
    mov x2, #3
    bl interrupt_handler
    ldr x2, [sp]
    add sp, sp, #8
    ldp x0, x1, [sp]
    add sp, sp, #16
    bl interrupt_enable
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
interrupt_enable:
    msr daifclr, #2
    ret
interrupt_disable:
    msr daifset, #2
    ret
