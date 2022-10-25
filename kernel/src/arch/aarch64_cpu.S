.global cpu_registers_save, cpu_registers_load, cpu_get_tick, __memset_64b, __memset_128b, __memcpy_64b, __memcpy_128b, cpu_get_cntpct, task_cpu_registers_load, task_cpu_registers_reload
//void __memset_64b(void *ptr = x0, uint64_t b = x1, uint64_t sz = x2)
__memset_64b:
    __memset_64b_loop:
    str  x1, [x0]
    add  x0, x0 ,#8
    sub  x2, x2, #8
    cbnz x2, __memset_64b_loop
    ret
//void __memset_128b(void *ptr = x0, uint64_t b[2] = x1, uint64_t sz = x2)
__memset_128b:
    ldp x3, x4, [x1]
    __memset_128b_loop:
    stp  x3, x4, [x0], #16
    sub  x2, x2, #16
    cbnz x2, __memset_128b_loop
    ret
//void __memcpy_64b(void *dst = x0, void *src = x1, uint64_t sz = x2)
__memcpy_64b:
    __memcpy_64b_loop:
    ldr  x3, [x1]
    str  x3, [x0]
    add  x0, x0, #8
    add  x1, x1, #8
    sub  x2, x2, #8
    cbnz x2, __memcpy_64b_loop
    ret

//void __memcpy_128b(void *dst = x0, void *src = x1, uint64_t sz = x2)
__memcpy_128b:
    __memcpy_128b_loop:
    ldp  x3, x4, [x1], #16
    stp  x3, x4, [x0], #16
    sub  x2, x2, #16
    cbnz x2, __memcpy_128b_loop
    ret
    
#define cpu_registers_x0  0
#define cpu_registers_x1  8
#define cpu_registers_x2  16
#define cpu_registers_x3  24
#define cpu_registers_x4  32
#define cpu_registers_x5  40
#define cpu_registers_x6  48
#define cpu_registers_x7  56
#define cpu_registers_x8  64
#define cpu_registers_x9  72
#define cpu_registers_x10 80
#define cpu_registers_x11 88
#define cpu_registers_x12 96
#define cpu_registers_x13 104
#define cpu_registers_x14 112
#define cpu_registers_x15 120
#define cpu_registers_x16 128
#define cpu_registers_x17 136
#define cpu_registers_x18 144
#define cpu_registers_x19 152
#define cpu_registers_x20 160
#define cpu_registers_x21 168
#define cpu_registers_x22 176
#define cpu_registers_x23 184
#define cpu_registers_x24 192
#define cpu_registers_x25 200
#define cpu_registers_x26 208
#define cpu_registers_x27 216
#define cpu_registers_x28 224
#define cpu_registers_x29 232
#define cpu_registers_x30 240
#define cpu_registers_sp  248

//void cpu_registers_save(cpu_registers *regs);
cpu_registers_save:
    str x1,  [x0, #cpu_registers_x1 ]
    str x2,  [x0, #cpu_registers_x2 ]
    str x3,  [x0, #cpu_registers_x3 ]
    str x4,  [x0, #cpu_registers_x4 ]
    str x5,  [x0, #cpu_registers_x5 ]
    str x6,  [x0, #cpu_registers_x6 ]
    str x7,  [x0, #cpu_registers_x7 ]
    str x8,  [x0, #cpu_registers_x8 ]
    str x9,  [x0, #cpu_registers_x9 ]
    str x10, [x0, #cpu_registers_x10]
    str x11, [x0, #cpu_registers_x11]
    str x12, [x0, #cpu_registers_x12]
    str x13, [x0, #cpu_registers_x13]
    str x14, [x0, #cpu_registers_x14]
    str x15, [x0, #cpu_registers_x15]
    str x16, [x0, #cpu_registers_x16]
    str x17, [x0, #cpu_registers_x17]
    str x18, [x0, #cpu_registers_x18]
    str x19, [x0, #cpu_registers_x19]
    str x20, [x0, #cpu_registers_x20]
    str x21, [x0, #cpu_registers_x21]
    str x22, [x0, #cpu_registers_x22]
    str x23, [x0, #cpu_registers_x23]
    str x24, [x0, #cpu_registers_x24]
    str x25, [x0, #cpu_registers_x25]
    str x26, [x0, #cpu_registers_x26]
    str x27, [x0, #cpu_registers_x27]
    str x28, [x0, #cpu_registers_x28]
    str x29, [x0, #cpu_registers_x29]
    str x30, [x0, #cpu_registers_x30]
    str x0,  [x0, #cpu_registers_x0 ]
    ret
//void cpu_registers_load(cpu_registers *regs);
cpu_registers_load:
    ldr x1,  [x0, #cpu_registers_x1 ]
    ldr x2,  [x0, #cpu_registers_x2 ]
    ldr x3,  [x0, #cpu_registers_x3 ]
    ldr x4,  [x0, #cpu_registers_x4 ]
    ldr x5,  [x0, #cpu_registers_x5 ]
    ldr x6,  [x0, #cpu_registers_x6 ]
    ldr x7,  [x0, #cpu_registers_x7 ]
    ldr x8,  [x0, #cpu_registers_x8 ]
    ldr x9,  [x0, #cpu_registers_x9 ]
    ldr x10, [x0, #cpu_registers_x10]
    ldr x11, [x0, #cpu_registers_x11]
    ldr x12, [x0, #cpu_registers_x12]
    ldr x13, [x0, #cpu_registers_x13]
    ldr x14, [x0, #cpu_registers_x14]
    ldr x15, [x0, #cpu_registers_x15]
    ldr x16, [x0, #cpu_registers_x16]
    ldr x17, [x0, #cpu_registers_x17]
    ldr x18, [x0, #cpu_registers_x18]
    ldr x19, [x0, #cpu_registers_x19]
    ldr x20, [x0, #cpu_registers_x20]
    ldr x21, [x0, #cpu_registers_x21]
    ldr x22, [x0, #cpu_registers_x22]
    ldr x23, [x0, #cpu_registers_x23]
    ldr x24, [x0, #cpu_registers_x24]
    ldr x25, [x0, #cpu_registers_x25]
    ldr x26, [x0, #cpu_registers_x26]
    ldr x27, [x0, #cpu_registers_x27]
    ldr x28, [x0, #cpu_registers_x28]
    ldr x29, [x0, #cpu_registers_x29]
    ldr x30, [x0, #cpu_registers_x30]
    ldr x0,  [x0, #cpu_registers_x0 ]
    ret

//uint64_t cpu_get_tick()
cpu_get_tick:
    mrs x0, CNTVCT_EL0
    ret
//uint64_t cpu_get_cntpct()
cpu_get_cntpct:
    mrs x0, CNTPCT_EL0
    ret
//void task_cpu_registers_load(cpu_registers *x0, void *x1 (fn), void *x2 (data), task *x3 (t));
task_cpu_registers_load:
    ldr x9,  [x0, #cpu_registers_sp ]
    mov sp,  x9
    ldr x29, [x0, #cpu_registers_x29] //rbp
    mov x9, x1
    mov x0, x2
    mov x1, x3
    msr daifset, #2 //interrupt enable
    br  x9
//void task_cpu_registers_reload(cpu_registers *regs x0);
task_cpu_registers_reload:
    msr daifset, #2 //interrupt enable
    ret
