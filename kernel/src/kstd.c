#include <kstd.h>

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
    int mod = sz % 16;
    switch(mod){
        case 0:
            for(uint64_t i = 0; i < sz; i += 16)
                ((__uint128_t *)ptr)[i >> 4] = (__uint128_t)b;
            return;
            break;
        case 8:
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
    int mod = sz % 16;
    switch(mod){
        case 0:
            for(uint64_t i = 0; i < sz; i += 16)
                ((__uint128_t *)dst)[i >> 4] = ((__uint128_t *)src)[i >> 4];
            return;
            break;
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
    int mod = sz % 16;
    switch(mod){
        case 0:
            for(uint64_t i = 0; i < sz; i += 16){
                if( ((__uint128_t *)ptr1)[i >> 4] != ((__uint128_t *)ptr2)[i >> 4] )
                    return -1;
            }
            break;
        case 8:
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
                if(((uint8_t *)ptr1)[i] != ((uint8_t *)ptr2)[i])
                    return -1;
            }
            break;
    } 
    return 0;
}
void * kmalloc(uint32_t b){
    return k_heapLCABAlloc(&HEAP, b);
}

void kfree(void *p){
    k_heapLCABFree(&HEAP, p);
}
