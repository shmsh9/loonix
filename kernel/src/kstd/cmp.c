#include <kstd/cmp.h>

bool cmp_char(char a, char b){
    return a == b;
}
bool cmp_str(const char *a, const char *b){
    return !(bool)strcmp(a,b);
}
bool cmp_u16(uint16_t a, uint16_t b){
    return a == b;
}
bool cmp_u32(uint32_t a, uint32_t b){
    return a == b;
}
