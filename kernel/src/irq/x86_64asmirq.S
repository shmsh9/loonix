.global irq_init_vectors, irq_disable, irq_enable
irq_init_vectors:
    ret
irq_enable:
    sti
    ret
irq_disable:
    cli
    ret
