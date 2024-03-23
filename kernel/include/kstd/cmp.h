#ifndef CMP_H_
#define CMP_H_
#include <kstd/kstd.h>

bool cmp_str(const char *a, const char *b);
static inline bool cmp_char(char a, char b){
    return a == b;
}
static inline bool cmp_u16(uint16_t a, uint16_t b){
    return a == b;
}
static inline bool cmp_u32(uint32_t a, uint32_t b){
    return a == b;
}

#endif
