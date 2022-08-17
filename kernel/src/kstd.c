#include <kstd.h>
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
    if(!l)
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
    serial_device_putchar(&serial, c);
    vt100_console_putchar(&fb,c);
}
char kgetchar(){
    if(ps2.data_port){
        return ps2_device_getchar(&ps2);
    }
    return serial_device_readchar(&serial);
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
            if(fmt[i+1] == 'd' || fmt[i+1] == 'x'){
                uint64_t n = __builtin_va_arg(arg, uint64_t);
                uint8_t base = fmt[i+1] == 'd' ? 10 : 16;
                char strn[32] = {0};
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
    int mod = sz % 16;
    switch(mod){
        case 0:
        {
            __uint128_t newb = (__uint128_t)b;
            uint64_t sz_octword = sz >> 4;
            for(uint64_t i = 0; i < sz_octword; i++){
                ((__uint128_t *)ptr)[i] = newb;
            }
            return;
            break;

        }
        case 8:
        {
            uint64_t sz_qword = (sz >> 3);
            uint64_t newb = (uint64_t)b;
            for(uint64_t i = 0; i < sz_qword; i ++){
                ((uint64_t *)ptr)[i] = newb;
            }
            return;
            break;
        }
        case 2:
        {
            uint64_t sz_word = (sz >> 1);
            uint16_t newb = (uint16_t)b;
            for(uint64_t i = 0; i < sz_word; i++)
                ((uint16_t *)ptr)[i] = newb;
            return;
            break;
        }
        case 4:{
            uint64_t sz_dword = (sz >> 2);
            uint32_t newb = (uint32_t)b;
            for(uint64_t i = 0; i < sz_dword; i++)
                ((uint32_t *)ptr)[i] = newb;
            return;
            break;
        }
        default:
            for(uint64_t i = 0; i < sz; i++)
                ((uint8_t *)ptr)[i] = b;
            return;
            break;
    }
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
    int mod = sz % 16;
    switch(mod){
        case 0:{
            uint64_t sz_octword = sz >> 4;
            for(uint64_t i = 0; i < sz_octword; i++){
                ((__uint128_t *)dst)[i] = ((__uint128_t *)src)[i];
            }
            return;
            break;
        }

        case 8:
            for(uint64_t i = 0; i < sz; i += 8)
                ((uint64_t *)dst)[i >> 3] = ((uint64_t *)src)[ i >> 3];
            return;
            break;
        case 2: 
            for(uint64_t i = 0; i < sz; i+=2)
                ((uint16_t *)dst)[i >> 1] = ((uint16_t *)src)[i >> 1];
            return;
            break;
        case 4:
            for(uint64_t i = 0; i < sz; i+=4)
                ((uint32_t *)dst)[i >> 2] = ((uint32_t *)src)[i >> 2];
            return;
            break;
        default:
            for(uint64_t i = 0; i < sz; i++)
                ((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
            return;
            break;
    }
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
    for(int i = 0; i < KALLOC_LIST_MAX; i++){
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
    KERROR("KALLOC_LIST_MAX !");
    return 0x0;
}
int32_t kalloc_find_ptr_alloc(const void *ptr){
    if(!ptr){
        KERROR("0x%x not in allocation table !", ptr);
        return -1;
    }
    for(int i = 0; i < KALLOC_LIST_MAX; i++){
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

karray *karray_new(uint8_t elementsz){
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
    ret->elementsz = elementsz; 
    ret->length = 0;
    ret->alloc = 16;
    ret->array = kmalloc(ret->elementsz*ret->alloc);
    return ret;
}
void karray_free(karray *array){
    kfree(array->array);
    array->array = 0x0;
    array->length = 0x0;
    array->alloc = 0x0;
    array->elementsz = 0x0;
    kfree(array);
}
void karray_print(karray *array){
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
