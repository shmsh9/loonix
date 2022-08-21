.global cpu_registers_save, cpu_registers_load, cpu_get_tick, __memset_64b, __memset_128b, __memcpy_64b, __memcpy_128b
//void __memset_128b(void *ptr = x0, uint64_t b = x1, uint64_t b2 = x2, uint64_t sz = x3)
__memset_128b:
    __memset_128b_loop:
    stp  x1, x2, [x0]
    add  x0, x0, #16
    sub  x3, x3, #16
    cbnz x3, __memcpy_128b_loop
    ret
//void __memset_64b(void *ptr = x0, uint64_t b = x1, uint64_t sz = x2)
__memset_64b:
    __memset_64b_loop:
    str  x1, [x0]
    add  x0, x0 ,#8
    sub  x2, x2, #8
    cbnz x2, __memset_64b_loop
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
    ldp  x3, x4, [x1]
    stp  x3, x4, [x0]
    add  x0, x0, #16
    add  x1, x1, #16
    sub  x2, x2, #16
    cbnz x2, __memcpy_128b_loop
    ret

//void cpu_registers_save(cpu_registers *regs);
cpu_registers_save:
    str x0,  [x0]
    str x1,  [x0, #8  ]
    str x2,  [x0, #16 ]
    str x3,  [x0, #24 ]
    str x4,  [x0, #32 ]
    str x5,  [x0, #40 ]
    str x6,  [x0, #48 ]
    str x7,  [x0, #56 ]
    str x8,  [x0, #64 ]
    str x9,  [x0, #72 ]
    str x10, [x0, #80 ]
    str x11, [x0, #88 ]
    str x12, [x0, #96 ]
    str x13, [x0, #104]
    str x14, [x0, #112 ]
    str x15, [x0, #120 ]
    str x16, [x0, #128 ]
    str x17, [x0, #136 ]
    str x18, [x0, #144 ]
    str x19, [x0, #152 ]
    str x20, [x0, #160 ]
    str x21, [x0, #168 ]
    str x22, [x0, #176 ]
    str x23, [x0, #184 ]
    str x24, [x0, #192 ]
    str x25, [x0, #200 ]
    str x26, [x0, #208 ]
    str x27, [x0, #216 ]
    str x28, [x0, #224 ]
    str x29, [x0, #232 ]
    str x30, [x0, #240 ]
    ret
//void cpu_registers_load(cpu_registers *regs);
cpu_registers_load:
    ldr x0,  [x0]
    ldr x1,  [x0, #8  ]
    ldr x2,  [x0, #16 ]
    ldr x3,  [x0, #24 ]
    ldr x4,  [x0, #32 ]
    ldr x5,  [x0, #40 ]
    ldr x6,  [x0, #48 ]
    ldr x7,  [x0, #56 ]
    ldr x8,  [x0, #64 ]
    ldr x9,  [x0, #72 ]
    ldr x10, [x0, #80 ]
    ldr x11, [x0, #88 ]
    ldr x12, [x0, #96 ]
    ldr x13, [x0, #104]
    ldr x14, [x0, #112 ]
    ldr x15, [x0, #120 ]
    ldr x16, [x0, #128 ]
    ldr x17, [x0, #136 ]
    ldr x18, [x0, #144 ]
    ldr x19, [x0, #152 ]
    ldr x20, [x0, #160 ]
    ldr x21, [x0, #168 ]
    ldr x22, [x0, #176 ]
    ldr x23, [x0, #184 ]
    ldr x24, [x0, #192 ]
    ldr x25, [x0, #200 ]
    ldr x26, [x0, #208 ]
    ldr x27, [x0, #216 ]
    ldr x28, [x0, #224 ]
    ldr x29, [x0, #232 ]
    ldr x30, [x0, #240 ]
    ret

//uint64_t cpu_get_tick()
cpu_get_tick:
    mrs x0, CNTVCT_EL0
    ret
