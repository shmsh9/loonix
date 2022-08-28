//https://krinkinmu.github.io/2021/01/10/aarch64-interrupt-handling.html
.text
.global interrupt_vector_table, init_interrupt_vector_table
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
    b interrupt_handler
.balign 0x80
    b interrupt_handler
.balign 0x80
    b interrupt_handler
.balign 0x80
    b interrupt_handler
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
