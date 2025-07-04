#include <sys/timer.h>

karray *timer_event_list;
void timer_init(){
    timer_event_list = karray_new(sizeof(timer_event *), kfree);
}
void timer_interrupt_handler(){
    for(int i = 0; i < timer_event_list->length; i++){
        timer_event *ev = ((timer_event **)timer_event_list->array)[i];
        if(ev->last_fire+ev->frequency <= getuptime100s()){
            ev->fn(ev->data);
            ev->last_fire = getuptime100s();
        }
    }
}

timer_event *timer_event_new(void(*fn)(void *), void *data, uint64_t frequency){
    timer_event *ret = kmalloc(sizeof(timer_event));
    *ret = (timer_event){
        .uuid = cpu_get_tick(),
        .last_fire = 0,
        .frequency = frequency,
        .data = data,
        .fn = fn
    };
    karray_push(timer_event_list, (uintptr_t)ret);
    return ret;
}
