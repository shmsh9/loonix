#include <event_loop.h>

event_loop *event_loop_new(){
    return karray_new(
        sizeof(event_loop_event *),
        kfree
    );
}
void event_loop_free(event_loop *loop){
    karray_free(loop);
}
void event_loop_loop(event_loop *loop){
    if(!loop){
        KERROR("event_loop_list not yet initialized");
        return;
    }
    while(1){
        uint64_t event_loop_length = loop->length;
        for(uint64_t i = 0; i < event_loop_length; i++){
            uint64_t tmp_tick = cpu_get_tick();
            event_loop_event *tmp_event = *(((event_loop_event **)loop->array)+i);
            if(tmp_tick >= tmp_event->last_execution+tmp_event->frequency){
                tmp_event->last_execution = tmp_tick;
                tmp_event->fn(tmp_event->payload);
            }
            event_loop_length = loop->length;
        }
        if(loop->length == 0)
            break;
    }
    event_loop_free(loop);
}

void event_loop_add(event_loop *loop, void (*fn)(void *), void *payload, uint64_t frequency){
    if(!loop){
        KERROR("loop == NULL");
        return;
    }
    event_loop_event *tmp_event = kcalloc(sizeof(event_loop_event), 1);
    *tmp_event = (event_loop_event){
        .fn = fn,
        .payload = payload,
        .frequency = frequency,
        .uuid = cpu_get_tick()
    };
    karray_push(loop, (uint64_t)tmp_event);
}
void event_loop_remove(event_loop *loop, event_loop_event *ev){
    if(!loop){
        KERROR("loop == NULL");
        return;
    }
    for(uint64_t i = 0; i < loop->length; i++){
        event_loop_event *curr_ev = *(((event_loop_event **)loop->array)+i);
        if(curr_ev->uuid == ev->uuid){
            karray_pop(loop, i);
            break;
        }
    }
}
void event_loop_remove_by_function(event_loop *loop, void (*fn)(void *)){
    if(!loop){
        KERROR("loop == NULL");
        return;
    }
    KDEBUG("removing fn 0x%x", fn);
    for(uint64_t i = 0; i < loop->length; i++){
        event_loop_event *curr_ev = *(((event_loop_event **)loop->array)+i);
        if(curr_ev->fn == fn){
            karray_pop(loop, i);
            break;
        }
    }

}
