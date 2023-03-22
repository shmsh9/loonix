#include <kstd/klist.h>
void klist_lock(klist *l){
    while(l->lock){
        sleep_100(1);
    }
    l->lock++;
}

void klist_unlock(klist *l){
    l->lock--;
}

klist *klist_new(void(*klist_data_free_fn)(void *)){
    klist *ret = kmalloc(sizeof(klist));
    *ret = (klist){
        .first = 0x0,
        .last = 0x0,
        .klist_data_free_fn = klist_data_free_fn,
        .lock = 0
    };
    return ret;
}

void klist_push(klist *k, uintptr_t data){
    if(!k){
        KERROR("k == NULL");
        return;
    }
    klist_lock(k);
    if(!k->first){
        klist_element *to_push = kmalloc(sizeof(klist_element));
        *to_push = (klist_element){
            .data = data,
            .next = 0x0,
            .prev = 0x0
        };
        k->first = to_push;
        k->last = to_push;
        klist_unlock(k);
        return;
    }
    klist_element *to_push = kmalloc(sizeof(klist_element));
    *to_push = (klist_element){
        .data = data,
        .next = k->first,
        .prev = 0x0
    };
    k->first->prev = to_push;
    k->first = to_push;
    klist_unlock(k);
}
void klist_free(klist *k){
    if(!k){
        KERROR("k == NULL");
        return;
    }
    klist_lock(k);
    if(!k->first){
        KERROR("k->first == NULL");
        return;
    }
    if(!k->last){
        KERROR("k->last == NULL");
        return;
    }
    klist_element *curr = k->first;
    while(curr){
        if(k->klist_data_free_fn)
            k->klist_data_free_fn((void *)curr->data);
        klist_element *prev = curr;
        curr = curr->next;
        kfree(prev);
    }
    klist_unlock(k);
    kfree(k);
}

void klist_debug_print(klist *k){
    if(!k){
        KERROR("k == NULL");
        return;
    }
    if(!k->first){
        KERROR("k->first == NULL");
        return;
    }
    if(!k->last){
        KERROR("k->last == NULL");
        return;
    }
    klist_lock(k);
    klist_element *curr = k->first;
    kprint("{");
    while(curr){
        kprintf(" 0x%x ->", curr->data);
        curr = curr->next;
    }
    kprint(" }\n");
    klist_unlock(k);
}
