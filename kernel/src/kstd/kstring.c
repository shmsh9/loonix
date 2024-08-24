#include <kstd/kstring.h>
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
inline uint8_t toupper(uint8_t c){
    if(c > 96 && c < 123)
        return c - 32;
    return c;
}
inline uint8_t tolower(uint8_t c){
    if(c > 64 && c < 91)
        return c + 32;
    return c;
}
inline char *itoa(int64_t v){
    if(v == 0){
        char *ret = kmalloc(sizeof(char)*2);
        ret[1] = 0x0;
        ret[0] = '0';
        return ret;
    }
    int l = v > 0 ? 0 : 1; //account for -;
    int64_t v_tmp = v;
    while(v_tmp){
        v_tmp /= 10;
        l++;
    }
    char *str = kmalloc(sizeof(char)*(l+1));
    str[l] = 0x0;
    if(v < 0){
        str[0] = '-';
    }
    v_tmp = v > 0 ? v : v*-1;
    int i = l-1;
    while(v_tmp && i > -1){
        str[i--] = (v_tmp%10) + '0';
        v_tmp /= 10;
    }
    return str;
}
inline int atoi(const char *str){
    if(str[0] == '0' && str[1] == 0x0){
        return 0;
    }
    int ret = 0;
    bool neg = str[0] == '-';
    str += neg;
    while(*str == '0')
        str++;

    while(*str){
        int c = isdigit(*str);
        if(c == 0 && *str != '0'){
            return -1;
        }
        ret *= 10;
        ret += c;
        str++;
    }
    return neg ? ret*-1 : ret;
}
inline int isdigit(uint8_t c){
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
char *_strdup(const char *str){
    vec v = vec_new(char);
    while(*str){
        vec_push(v, *str);
	str++;
    }
    vec_push(v, 0x0);
    char *ret = v->_array;
    kfree(v);
    return ret;
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

char *strings_join(char **s, int n, char j){
    uint32_t *l = kmalloc(sizeof(uint32_t)*n);
    uint32_t sz = 0;
    for(int i = 0; i < n; i++){
        l[i] = strlen(s[i]);
        sz += l[i] + (j && i+1 != n ? 1 : 0);
    }
    char *ret = kmalloc(sz+1);
    ret[sz] = 0x0;
    char *ret_curr = ret;
    for(int i = 0; i < n; i++){
        memcpy(ret_curr, s[i], l[i]);
        if(j && i+1 != n)
            ret_curr[l[i]] = j;
        ret_curr += l[i] + (j ? 1 : 0);
    }
    kfree(l);
    return ret;
}
char *string_remove(char *s, char c){
    int l = strlen(s);
    int sz = l;
    for(int i = 0; i < l; i++)
        sz = s[i] == c ? sz-1 : sz;
    char *ret = kmalloc(sz+1);
    char *curr_ret = ret;
    ret[sz] = 0x0;
    for(int i = 0; i < l; i++){
        if(s[i] != c)
            *curr_ret++ = s[i];
    }
    return ret;
}
void string_replace(char *s, char c, char r){
    while(*s){
        *s = *s == c ? r : *s;
        s++;
    }
}
