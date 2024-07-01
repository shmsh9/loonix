#include <kstd/kstd.h>
uint64_t kalloc_list_alloc = KALLOC_LIST_START_ALLOC;
uint64_t kalloc_list_last = 0;

__attribute__((noreturn))
void __stack_chk_fail(void){
    KPANIC("0x%x", __stack_chk_guard);
    while(1){}
}
void kprint(const char *str){
	  if(!str)
        return;
    while(*str)
		kputc(*str++);
}
void kputc(uint8_t c){
    if(serial)
        serial_device_putchar(serial, c);
    if(vt100_console_initialized)
        vt100_console_putchar(fb,c);
}
char kgetchar(){
    if(ps2)
        return ps2_device_getchar(ps2);
    if(serial) 
        return serial_device_readchar(serial);
    return 0;
}
ASYNC_FN(kgetchar_async, char *out, {
    char c = kgetchar_non_blocking();
    if(!c)
        ASYNC_YIELD();
    *out = c;
    ASYNC_OVER();
})
char kgetchar_non_blocking(){
    char ret = 0;
    if(ps2)
        ret = ps2_device_getchar_non_blocking(ps2);
    if(ret)
        return ret;
    if(serial)
        ret = serial_device_readchar_non_blocking(serial);
    if(ret)
        return ret;
    return 0;
}
/* void kprintf(const char *fmt, ...){
    if(!fmt){
        KERROR("fmt == NULL");
        return;
    }
    __builtin_va_list arg;
    __builtin_va_start(arg, fmt); 
    int lfmt = strlen(fmt);
    const char hextable[] = "0123456789abcdef";
    for(int i = 0; i < lfmt; i++){
        if(fmt[i] == '%' && i+1 < lfmt){
            if(fmt[i+1] == 'd' || fmt[i+1] == 'x' || fmt[i+1] == 'b'){
                uint64_t n = __builtin_va_arg(arg, uint64_t);
                uint8_t base = 1;
                switch(fmt[i+1]){
                    case 'd':
                        base = 10;
                        break;
                    case 'x':
                        base = 16;
                        break;
                    case 'b':
                        base = 2;
                        break;
                }
                char strn[65] = {0};
                char *ptr = strn;
                int digits = 0;
                if(n == 0)
                    strn[0] = '0';
                while(n){
                    *ptr = hextable[n%base];
                    ptr++;
                    digits++;
                    n /= base;
                }
                for(int j = 0; j < digits; j++, digits--){
                    char tmp = strn[j];
                    strn[j] = strn[digits-1];
                    strn[digits-1] = tmp;
                }
                kprint(strn);
                i += 1;
                continue;
            }
            if(fmt[i+1] == 's'){
                char *tmps = __builtin_va_arg(arg, char*);
                kprint(tmps);
                i += 1;
                continue;
            }
            if(fmt[i+1] == 'c'){
                //                                  Hum....
                kputc((char)__builtin_va_arg(arg, uint32_t));
                i += 1;
                continue;
            }
        }
        kputc(fmt[i]);
    }
    __builtin_va_end(arg);
}
*/

void kprinthex(void *ptr, uint64_t n){
    #define KPRINTHEX_PADDING(n) (n < 0x10 ? "0":"")
    if(!ptr)
        return;
    uint64_t counter = (uint64_t)ptr;
    uint8_t *ptr_casted = (uint8_t *)ptr;
    for(uint64_t j = 0; j < n; j+= 8, counter+= 8){
        kprintf("0x%x: ", counter);
        for(uint64_t i = 0; i < 8 && i+j < n; i++){
            kprintf("0x%s%x ", KPRINTHEX_PADDING(ptr_casted[j+i]), ptr_casted[j+i]);
        }
        kputc(' ');
        for(uint64_t i = 0; i < 8 && i+j < n; i++){
            if(ptr_casted[j+i] < 128 && ptr_casted[j+i] > 31)
                kprintf("%c ", ptr_casted[j+i]);
            else
                kprint(". ");
        }
        kputc('\n');
    }
    kprint("\n");
}
void *kmalloc(uint64_t b){
    task_lock();
    if(!b){
        KERROR("b == 0");
        task_unlock();
        return 0x0;
    }
    for(int i = kalloc_list_last; i < kalloc_list_alloc; i++){
        if(kalloc_list[i].ptr == 0){
            kheap_allocated_block block = kheap_get_free_mem2(&heap, b);
            kalloc_list[i] = block;
            if(block.ptr){
                kalloc_list_last = i;
                task_allocation_add(kalloc_list+i);
                task_unlock();
                return (void *)block.ptr;
            }
            else{
                KERROR("allocation failed !");
                task_unlock();
                return 0x0;
            }
        }
    }
    KDEBUG("last allocation slot was not free", 0x0);
    for(int i = 0; i < kalloc_list_last; i++){
        if(kalloc_list[i].ptr == 0){
            kheap_allocated_block block = kheap_get_free_mem2(&heap, b);
            kalloc_list[i] = block;
            if(block.ptr){
                kalloc_list_last = i;
                task_allocation_add(kalloc_list+i);
                task_unlock();
                return (void *)block.ptr;
            }
            else{
                KERROR("allocation failed !");
                task_unlock();
                return 0x0;
            }
        }
    }

    //If no more alloc list
    uint64_t kalloc_list_alloc_new = kalloc_list_alloc*2;
    uint64_t kalloc_list_alloc_old = kalloc_list_alloc;
    kheap_allocated_block tmp = kheap_get_free_mem2(&heap, kalloc_list_alloc_new*sizeof(kheap_allocated_block));
    if(!tmp.ptr){
        KERROR("not enough memory to realloc kalloc_list !");
        task_unlock();
        return 0x0;
    }
    KDEBUG("reallocating kalloc_list to %d", kalloc_list_alloc_new);
    memcpy((void *)tmp.ptr, (void *)kalloc_list, kalloc_list_alloc_old*sizeof(kheap_allocated_block));
    memset((kheap_allocated_block *)tmp.ptr+kalloc_list_alloc_old, 0, kalloc_list_alloc_old*sizeof(kheap_allocated_block));
    //need to free previous kalloc_list
    kheap_free_mem2(&heap, &kalloc_list_block);
    kalloc_list_alloc = kalloc_list_alloc_new;
    kalloc_list_block = tmp;
    kalloc_list = (kheap_allocated_block *)tmp.ptr;
    for(int i = kalloc_list_alloc_old; i < kalloc_list_alloc; i++){
        if(kalloc_list[i].ptr == 0){
            kheap_allocated_block block = kheap_get_free_mem2(&heap, b);
            kalloc_list[i] = block;
            if(block.ptr){
                task_allocation_add(kalloc_list+i);
                task_unlock();
                return (void *)block.ptr;
            }
            else{
                KERROR("allocation failed !");
                task_unlock();
                return 0x0;
            }
        }
    }
    KERROR("kmalloc failed even after resizing");
    task_unlock();
    return 0x0;
}
int32_t kalloc_find_ptr_alloc(const void *ptr){
    if(!ptr){
        KERROR("ptr == NULL !", ptr);
        return -1;
    }
    task_lock();
    for(uint64_t i = kalloc_list_last; i < kalloc_list_alloc; i++){
        if(kalloc_list[i].ptr == (uintptr_t)ptr){
            task_unlock();
            return i;
        }
    }
    for(uint64_t i = 0; i < kalloc_list_last; i++){
        if(kalloc_list[i].ptr == (uintptr_t)ptr){
            task_unlock();
            return i;
        }
    }
    //KERROR("0x%x not in allocation table !", ptr);
    task_unlock();
    return -1;
}
void *kcalloc(uint64_t n, uint64_t sz){
    task_lock();
    if(n == 0){
        KERROR("n == 0");
        task_unlock();
        return 0x0;
    }
    if(sz == 0){
        KERROR("sz == 0");
        task_unlock();
        return 0x0;
    }
    void *ret = kmalloc(n*sz);
    if(!ret){
        KERROR("kmalloc() failed")
        task_unlock();
        return 0x0;
    }
    memset(ret, 0, n*sz);
    task_unlock();
    return ret;
}
void *krealloc(const void *ptr, uint64_t newsz){
    task_lock();
    if(!ptr){
        KERROR("ptr == NULL");
        task_unlock();
        return 0x0;
    }
    if(!newsz){
        KERROR("newsz == 0");
        task_unlock();
        return 0x0;
    }
    int32_t oldptr = kalloc_find_ptr_alloc(ptr);
    if(oldptr == -1){
        task_unlock();
        return 0x0;
    }
    void *ret = kmalloc(newsz);
    if(!ret){
        task_unlock();
        return 0x0;
    }
    memcpy(ret, ptr, (uint64_t)kalloc_list[oldptr].size);
    task_unlock();
    return ret; 
}
void kfree(void *p){
    task_lock();
    if(!p){
        KERROR("null pointer !");
        task_unlock();
        return;
    }
    int32_t ptrindex = kalloc_find_ptr_alloc(p);
    if(ptrindex == -1){
        task_unlock();
        return;
    }
    /*
    if(task_current && kalloc_list[ptrindex].task != task_current){
        KERROR("kalloc_list[ptrindex].task (%s) != task_current (%s)", 
            kalloc_list[ptrindex].task->name,
            task_current->name
        );
        task_unlock();
        return;
    }
    */
    kheap_free_mem2(&heap, &kalloc_list[ptrindex]);
    kalloc_list[ptrindex] = (kheap_allocated_block){
        .bit = 0,
        .bitfield = 0,
        .block = 0,
        .ptr = 0,
        .size = 0,
        .task = 0
    };
    task_unlock();
}
