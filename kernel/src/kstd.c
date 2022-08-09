#include <kstd.h>

__attribute__((noreturn))
void __stack_chk_fail(void){
	kprint("stack_chk_fail() !");
	BREAKPOINT();
	while(1){}
}

int strlen(const char *str){
    int r = 0;
    while(*str++)
        r++;
    return r;
}
int strcmp(const char *str1, const char *str2){
    int l1 = strlen(str1);
    int l2 = strlen(str2);
    if(l1 != l2)
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
	while(*str)
		SERIAL_PUTCHAR(*str++);
}
void kputc(uint8_t c){
    SERIAL_PUTCHAR(c);
}
char kgetchar(){
    return SERIAL_READCHAR();
}
void kprintf(const char *fmt, ...){
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
        SERIAL_PUTCHAR(fmt[i]);
    }
    __builtin_va_end(arg);
}

void memset(void *ptr, uint8_t b, uint64_t sz){
    int mod = sz % 8;
    //kprintf("memset() mod == 0x%x\n", mod);
    switch(mod){
        case 0:
            for(uint64_t i = 0; i < sz; i += 8)
                ((uint64_t *)ptr)[i >> 3] = (uint64_t)b;
            return;
            break;
        case 2: 
            for(uint64_t i = 0; i < sz; i+=2)
                ((uint16_t *)ptr)[i >> 1] = (uint16_t)b;
            return;
            break;
        case 4:
            for(uint64_t i = 0; i < sz; i+=4)
                ((uint32_t *)ptr)[i >> 2] = (uint32_t)b;
            return;
            break;
        default:
            for(uint64_t i = 0; i < sz; i++)
                ((uint8_t *)ptr)[i] = b;
            return;
            break;
    }
}
void memcpy(void *dst, const void *src, uint64_t sz){
    int mod = sz % 8;
    switch(mod){
        case 0:
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
void *kmalloc(uint32_t b){
    for(int i = 0; i < KALLOC_LIST_MAX; i++){
        if(kalloc_list[i].ptr == 0){
            kheap_allocated_block block = kheap_get_free_mem(&heap, b);
            kalloc_list[i] = block;
            if(block.ptr){
                return (void *)block.ptr;
            }
            else{
                kprint("kmalloc() : failed !\n");
                return 0x0;
            }
        }
    }
    kprint("kmalloc() : KALLOC_LIST_MAX !\n");
    return 0x0;
}
void *kcalloc(uint32_t n, uint32_t sz){
    void *ret = kmalloc(n*sz);
    memset(ret, 0, n*sz);
    return ret;
}
void *krealloc(const void *ptr, uint32_t oldsz , uint32_t newsz){
    //kprintf("krealloc() *ptr == 0x%x oldsz == %d && newsz == %d\n", ptr, oldsz, newsz);
    void *ret = kmalloc(newsz);
    //kprintf("krealloc() after ret == 0x%x\n", ret);
    if(!ret)
        return 0x0;
    //possible buffer nopeoverrun
    /* 
    for(int i = 0; i < oldsz; i++){
        ((uint8_t *)ret)[i] = ((uint8_t *)ptr)[i]; 
    }
    */
    memcpy(ret, ptr, (uint64_t)oldsz);
    return ret; 
}
void kfree(void *p){
    if(!p){
        kprint("kfree() : null pointer !\n");
        return;
    }
    for(uint64_t i = 0; i < KALLOC_LIST_MAX; i++){
        if(kalloc_list[i].ptr == (uintptr_t)p){
            kheap_free_mem(&kalloc_list[i]);
            memset(kalloc_list+i, 0, sizeof(kheap_allocated_block));
            return;
        }
    }
    kprint("kfree() : pointer not allocated !\n");
}

void karray_push(karray *array, uint64_t elem){
    if(array->length+1 > array->alloc){
        void *tmp = krealloc(array->array, array->alloc*array->elementsz, ((array->alloc*array->elementsz)<<1));
        //kprintf("krealloc(%d, %d)\n", array->alloc*array->elementsz, ((array->alloc*array->elementsz)<<1));
        if(tmp){
            kfree(array->array);
            array->array = tmp;
            array->alloc <<= 1;
        }
        else{
            kprint("karray_push() : krealloc() : failure\n");
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
    //kprintf("karray_push() : elem == 0x%x array->alloc == %d array->array == 0x%x array->length == %d && array->elementsz == %d\n", elem,array->alloc, array->array, array->length, array->elementsz);
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
            kprintf("karray_new() : 0x%x is not a valid element size\n", elementsz);
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

