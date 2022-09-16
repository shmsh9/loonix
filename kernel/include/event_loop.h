#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <kstd.h>
typedef struct _event_loop_event{
    uint64_t frequency;
    uint64_t last_execution;
    uint64_t uuid;
    void *payload;
    void (*fn)(void *);
}event_loop_event;
void event_loop_init();
void event_loop_loop();
void event_loop_add(void (*fn)(void *), void *payload, uint64_t frequency);
void event_loop_remove(event_loop_event *ev);
#endif
