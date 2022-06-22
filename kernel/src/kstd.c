#include <kstd.h>

int strlen(const char *str){
    int r = 0;
    while(*str++)
        r++;
    return r;
}
void kprint(const char *str){
	while(*str){
		SERIAL_PUTCHAR(*str);
		str++;
	}
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
        }
        SERIAL_PUTCHAR(fmt[i]);
    }
    __builtin_va_end(arg);
}

void memset(void *ptr, uint8_t b, uint64_t sz){
    for(uint64_t i = 0; i < sz; i++)
        ((uint8_t *)ptr)[i] = b;
}
void memcpy(void *dst, const void *src, uint64_t sz){
    for(uint64_t i = 0; i < sz; i++)
        ((uint8_t *)dst)[i] = ((uint8_t *)src)[i];
}
