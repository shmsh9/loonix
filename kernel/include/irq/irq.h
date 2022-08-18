#ifndef IRQ_H_
#define IRQ_H_
#include <arch/aarch64.h>
#include <arch/x86_64.h>
void irq_init_vectors(void *vectors);
void irq_enable();
void irq_disable();
#endif
