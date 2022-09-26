#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_


#include <kstd.h>

typedef karray event_loop;

typedef struct _event_loop_event{
    uint64_t frequency;
    uint64_t last_execution;
    uint64_t uuid;
    void *payload;
    void (*fn)(void *);
    event_loop *loop;
}event_loop_event;
event_loop *event_loop_new();
void event_loop_loop(event_loop *loop);
void event_loop_loop_non_blocking(event_loop *loop);
void event_loop_add(event_loop *loop, void (*fn)(void *), void *payload, uint64_t frequency);
void event_loop_remove(event_loop *loop, event_loop_event *ev);
void event_loop_remove_by_function(event_loop *loop, void (*fn)(void *));
#endif
