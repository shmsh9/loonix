#ifndef TIMER_H_
#define TIMER_H_
#include <kstd.h>

typedef struct _timer_event{
    void (*fn)(void *);
    void *data;
    uint64_t frequency;
    uint64_t last_fire;
    uint64_t uuid;
}timer_event;
void timer_init();
void timer_interrupt_handler();
timer_event *timer_event_new(void(*fn)(void *), void *data);
#endif
