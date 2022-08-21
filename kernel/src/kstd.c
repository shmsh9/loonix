#include <kstd.h>
uint64_t kalloc_list_alloc = KALLOC_LIST_START_ALLOC;
uint64_t kalloc_list_last = 0;

void stacktrace(){
    struct stackframe *stk = {0};
    GET_STACKFRAME(stk);
    kprint("stacktrace : \n");
    for(uint8_t i = 0; stk && i < STACK_TRACE_NMAX; i++){
        kprintf("\t[%d] : 0x%x\n", i, stk->instruction_pointer);
        stk = stk->frame;
    }
}
__attribute__((noreturn))
void __stack_chk_fail(void){
    KPANIC("0x%x", __stack_chk_guard);
    while(1){}
}
int strlen(const char *str){
	  if(!str){
        KERROR("str == NULL");
        return -1;
    }
    int r = 0;
    while(*str++)
        r++;
    return r;
}
int atoi(const char *str){
    int ret = 0;
    while(*str++){
        int c = isdigit(*str);
        if(c == 0){
            return -1;
        }
        ret *= 10;
        ret += c;
    }
    return ret;
}
int isdigit(uint8_t c){
    int r = c - '0';
    if(r > 9 || r < 0){
        return 0;
    }
    return r;
}
int strcmp(const char *str1, const char *str2){
    int l1 = strlen(str1);
    int l2 = strlen(str2);
    if(l1 != l2 || l1 == -1 || l2 == -1)
        return -1;
    return memcmp(str1, str2, l1);
}
char *strdup(const char *str){
    int l = strlen(str);
    if(l == -1)
        return 0x0;
    char *ret = kmalloc(l+1);
    memcpy(ret, str, l+1);
    return ret;
}
void kprint(const char *str){
	  if(!str){
        KERROR("str == NULL");
        return;
    }
    while(*str)
		kputc(*str++);
}
void kputc(uint8_t c){
    serial_device_putchar(serial, c);
    vt100_console_putchar(&fb,c);
}
char kgetchar(){
    if(ps2){
        return ps2_device_getchar(ps2);
    }
    return serial_device_readchar(serial);
}
char kgetchar_non_blocking(){
    if(ps2)
        return ps2_device_getchar_non_blocking(ps2);
    if(serial)
        return serial_device_readchar_non_blocking(serial);
    return 0;
}
void kprintf(const char *fmt, ...){
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
                i += 2;
            }
            if(fmt[i+1] == 's'){
                char *tmps = __builtin_va_arg(arg, char*);
                kprint(tmps);
                i += 2;
            }
            if(fmt[i+1] == 'c'){
                //                                  Hum....
                kputc((char)__builtin_va_arg(arg, uint32_t));
                i += 2;
            }
        }
        kputc(fmt[i]);
    }
    __builtin_va_end(arg);
}
void memset(void *ptr, uint8_t b, uint64_t sz){
    if(!ptr){
        KERROR("ptr == NULL");
        return;
    }
    uint8_t *cp_ptr = (uint8_t *)ptr;
    while (sz % 16){
        *cp_ptr++ = b;
        sz--;
    }
    if(!sz)
        return;
    __FASTEST_MEMSET(cp_ptr, b, sz);
}
uint64_t B_to_8B(uint8_t b){
    uint64_t b64 = (uint64_t)b;
    return (uint64_t)(  
        b64 << 56 | b64 << 48 | b64 << 40 | b64 << 32|
        b64 << 24 | b64 << 16 | b64 << 8  | b64 
    );
}
void memcpy(void *dst, const void *src, uint64_t sz){
    if(!dst){
        KERROR("dst == NULL");
        return;
    }
    if(!src){
        KERROR("src == NULL");
        return;
    }
    uint8_t *cp_dst = (uint8_t *)dst;
    uint8_t *cp_src = (uint8_t *)src;

    while (sz % 16){
        *cp_dst++ = *cp_src++;
        sz--;
    }
    if(!sz)
        return;
    __FASTEST_MEMCPY(cp_dst, cp_src, sz);
}
int memcmp(const void *ptr1, const void *ptr2, uint64_t sz){
    if(!ptr1){
        KERROR("ptr1 == NULL");
        return -1;
    }
    if(!ptr2){
        KERROR("ptr2 == NULL");
        return -1;
    }
    int mod = sz % 8;
    switch(mod){
        case 0:
            for(uint64_t i = 0; i < sz; i += 8){
                if( ((uint64_t *)ptr1)[i >> 3] != ((uint64_t *)ptr2)[i >> 3] )
                    return -1;
            }
            break;
        case 2:
            for(uint64_t i = 0; i < sz; i += 2){
                if( ((uint16_t *)ptr1)[i >> 1] != ((uint16_t *)ptr2)[i >> 1] )
                    return -1;
            }
            break;
        case 4:
            for(uint64_t i = 0; i < sz; i += 4){
                if( ((uint32_t *)ptr1)[i >> 2] != ((uint32_t *)ptr2)[i >> 2] )
                    return -1;
            }
            break;
        default:
            for(uint64_t i = 0; i < sz; i++){
                if(((uint8_t *)ptr1)[i] != ((uint8_t *)ptr2)[i]){
                    return -1;
                }
            }
            break;
    } 
    return 0;
}
void *kmalloc(uint64_t b){
    if(!b){
        KERROR("b == 0");
        return 0x0;
    }
    for(int i = kalloc_list_last; i < kalloc_list_alloc; i++){
        if(kalloc_list[i].ptr == 0){
            kheap_allocated_block block = kheap_get_free_mem2(&heap, b);
            kalloc_list[i] = block;
            if(block.ptr){
                kalloc_list_last = i;
                return (void *)block.ptr;
            }
            else{
                KERROR("allocation failed !");
                return 0x0;
            }
        }
    }
    for(int i = 0; i < kalloc_list_last; i++){
        if(kalloc_list[i].ptr == 0){
            kheap_allocated_block block = kheap_get_free_mem2(&heap, b);
            kalloc_list[i] = block;
            if(block.ptr){
                kalloc_list_last = i;
                return (void *)block.ptr;
            }
            else{
                KERROR("allocation failed !");
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
        return 0x0;
    }
    //LEAKS
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
                return (void *)block.ptr;
            }
            else{
                KERROR("allocation failed !");
                return 0x0;
            }
        }
    }
    KERROR("kmalloc failed even after resizing");
    return 0x0;
}
int32_t kalloc_find_ptr_alloc(const void *ptr){
    if(!ptr){
        KERROR("0x%x not in allocation table !", ptr);
        return -1;
    }
    for(uint64_t i = kalloc_list_last; i < kalloc_list_alloc; i++){
        if(kalloc_list[i].ptr == (uintptr_t)ptr)
            return i;
    }
    for(uint64_t i = 0; i < kalloc_list_last; i++){
        if(kalloc_list[i].ptr == (uintptr_t)ptr)
            return i;
    }
    KERROR("0x%x not in allocation table !", ptr);
    return -1;
}
void *kcalloc(uint64_t n, uint64_t sz){
    if(n == 0){
        KERROR("n == 0");
        return 0x0;
    }
    if(sz == 0){
        KERROR("sz == 0");
        return 0x0;
    }
    void *ret = kmalloc(n*sz);
    if(!ret){
        KERROR("kmalloc() failed")
        return ret;
    }
    memset(ret, 0, n*sz);
    return ret;
}
void *krealloc(const void *ptr, uint64_t newsz){
    if(!ptr){
        KERROR("ptr == NULL");
        return 0x0;
    }
    if(!newsz){
        KERROR("newsz == 0");
        return 0x0;
    }
    int32_t oldptr = kalloc_find_ptr_alloc(ptr);
    if(oldptr == -1)
        return 0x0;
    void *ret = kmalloc(newsz);
    if(!ret)
        return 0x0;
    memcpy(ret, ptr, (uint64_t)kalloc_list[oldptr].size);
    return ret; 
}
void kfree(void *p){
    if(!p){
        KERROR("null pointer !");
        return;
    }
    int32_t ptrindex = kalloc_find_ptr_alloc(p);
    if(ptrindex == -1)
        return;
    kheap_free_mem2(&heap, &kalloc_list[ptrindex]);
    memset(kalloc_list+ptrindex, 0, sizeof(kheap_allocated_block));
}

void karray_push(karray *array, uint64_t elem){
    if(array->length+1 > array->alloc){
        void *tmp = krealloc(array->array, ((array->alloc*array->elementsz)<<1));
        if(tmp){
            kfree(array->array);
            array->array = tmp;
            array->alloc <<= 1;
        }
        else{
            KERROR("krealloc() : failed");
            return;
        }
    }
    switch(array->elementsz){
        case 1:
            ((uint8_t *)array->array)[array->length++] = (uint8_t)elem;
            break;
        case 2:
            ((uint16_t *)array->array)[array->length++] = (uint16_t)elem;
            break;
        case 4:
            ((uint32_t *)array->array)[array->length++] = (uint32_t)elem;
            break;
        case 8:
            ((uint64_t *)array->array)[array->length++] = (uint64_t)elem;
            break;
    }
}

karray *karray_new(uint8_t elementsz, void(*karray_data_free_fn)(void *)){
    switch(elementsz){
        case 1:
            break;
        case 2:
            break;
        case 4:
            break;
        case 8:
            break;
        default:
            KERROR("0x%x is not a valid element size", elementsz);
            return 0x0;
            break;
    }
    karray *ret = kmalloc(sizeof(karray));
    *ret = (karray){
        .elementsz = elementsz,
        .length = 0,
        .alloc = 16,
        .karray_data_free_fn = karray_data_free_fn
    };
    ret->array = kmalloc(ret->alloc*ret->elementsz);
    return ret;
}
void karray_free(karray *array){
    if(array->karray_data_free_fn){
        switch (array->elementsz){
            case 1:{
                uint8_t *casted_array = (uint8_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)(uintptr_t)casted_array[i]);
                }
                break;
            }
            case 2:{
                uint16_t *casted_array = (uint16_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)(uintptr_t)casted_array[i]);
                }
                break;
            }
            case 4:{
                uint32_t *casted_array = (uint32_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)(uintptr_t)casted_array[i]);
                }
                break;
            }
            case 8:{
                uint64_t *casted_array = (uint64_t *)array->array;
                for(int i = 0; i < array->length; i++){
                    array->karray_data_free_fn((void*)casted_array[i]);
                }
                break;
            }
        }
    }
    kfree(array->array);
    array->array = 0x0;
    array->length = 0x0;
    array->alloc = 0x0;
    array->elementsz = 0x0;
    kfree(array);
}
void karray_debug_print(karray *array){
    kprint("{ ");
    switch(array->elementsz){
        case 1:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint8_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
        case 2:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint16_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
        case 4:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint32_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
        case 8:
	          for(int i = 0; i < array->length; i++){
		            kprintf("0x%x ",((uint64_t *)array->array)[i]);
		            if(i+1 < array->length)
			              kputc(',');
	          }
            break;
    }
	  kprint("}\n");
}

klist *klist_new(void(*klist_data_free_fn)(void *)){
    klist *ret = kmalloc(sizeof(klist));
    *ret = (klist){
        .first = 0x0,
        .last = 0x0,
        .klist_data_free_fn = klist_data_free_fn
    };
    return ret;
}

void klist_push(klist *k, uintptr_t data){
    if(!k){
        KERROR("k == NULL");
        return;
    }
    if(!k->first){
        klist_element *to_push = kmalloc(sizeof(klist_element));
        *to_push = (klist_element){
            .data = data,
            .next = 0x0,
            .prev = 0x0
        };
        *k = (klist){
            .first = to_push,
            .last = to_push,
            .klist_data_free_fn = k->klist_data_free_fn,
        };
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
}

void klist_free(klist *k){
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
    klist_element *curr = k->first;
    if(!curr->next){
        if(k->klist_data_free_fn)
            k->klist_data_free_fn((void *)k->first->data);
        kfree(curr);
        kfree(k);
        return;
    }
    while(curr->next){
        klist_element *tmp = curr->next;
        if(k->klist_data_free_fn)
            k->klist_data_free_fn((void *)curr->data);
        kfree(curr);
        curr = tmp;
    }
    if(k->klist_data_free_fn)
        k->klist_data_free_fn((void *)curr->data);
    kfree(curr);
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
    klist_element *curr = k->first;
    if(!curr->next){
        kprintf("{ 0x%x }\n", curr->data);
        return;
    }
    kprint("{");
    while (curr->next){
        kprintf(" 0x%x ->", curr->data);
        curr = curr->next;
    }
    kprintf(" 0x%x }\n", curr->data);
}
