#ifndef IRQ_H_
#define IRQ_H_
void irq_init_vectors(void *vectors);
void irq_enable();
void irq_disable();
#endif
