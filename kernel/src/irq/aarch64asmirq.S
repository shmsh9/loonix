.global irq_init_vectors, irq_disable, irq_enable
//void irq_init_vectors(void *vectors)
irq_init_vectors:
    msr vbar_el1, x0
    ret
irq_enable:
    msr daifclr, #2
    ret
irq_disable:
    msr daifset, #2
    ret
