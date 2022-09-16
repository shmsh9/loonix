#ifndef EVENT_LOOP_H_
#define EVENT_LOOP_H_

#include <kstd.h>
extern karray *event_loop_list;
typedef struct _event_loop_event{
    uint64_t frequency;
    uint64_t last_execution;
    uint64_t uuid;
    void *payload;
    void (*fn)(void *);
}event_loop_event;

#endif
