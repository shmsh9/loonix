#include <event_loop.h>

karray *event_loop_list = 0x0;

void event_loop_init(){
    if(event_loop_list){
        KERROR("event_loo");
        return;
    }
    event_loop_list = karray_new(
        sizeof(event_loop_event *),
        kfree
    );
}
void event_loop_loop(){
    if(!event_loop_list){
        KERROR("event_loop_list not yet initialized");
        return;
    }
    while(1){
        uint64_t event_loop_length = event_loop_list->length;
        for(uint64_t i = 0; i < event_loop_length; i++){
            uint64_t tmp_tick = cpu_get_tick();
            event_loop_event *tmp_event = *(((event_loop_event **)event_loop_list->array)+i);
            if(tmp_tick >= tmp_event->last_execution+tmp_event->frequency){
                tmp_event->last_execution = tmp_tick;
                tmp_event->fn(tmp_event->payload);
            }
            event_loop_length = event_loop_list->length;
        }
        if(event_loop_list->length == 0)
            break;
    }
}

void event_loop_add(void (*fn)(void *), void *payload, uint64_t frequency){
    if(!event_loop_list){
        KERROR("event_loop_list not yet initialized");
    }
    event_loop_event *tmp_event = kcalloc(sizeof(event_loop_event), 1);
    *tmp_event = (event_loop_event){
        .fn = fn,
        .payload = payload,
        .frequency = frequency,
        .uuid = cpu_get_tick()
    };
    karray_push(event_loop_list, (uint64_t)tmp_event);
}

void event_loop_remove_by_uuid(event_loop_event *ev){
    if(!event_loop_list){
        KERROR("event_loop_list not yet initialized");
        return;
    }
    for(uint64_t i = 0; i < event_loop_list->length; i++){
        event_loop_event *curr_ev = *(((event_loop_event **)event_loop_list->array)+i);
        if(curr_ev->uuid == ev->uuid){
            karray_pop(event_loop_list, i);
            break;
        }
    }
}

void event_loop_remove_by_function(void (*fn)(void *)){
    if(!event_loop_list){
        KERROR("event_loop_list not yet initialized");
        return;
    }
    KDEBUG("removing fn 0x%x", fn);
    for(uint64_t i = 0; i < event_loop_list->length; i++){
        event_loop_event *curr_ev = *(((event_loop_event **)event_loop_list->array)+i);
        if(curr_ev->fn == fn){
            karray_pop(event_loop_list, i);
            break;
        }
    }

}
