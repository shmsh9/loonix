#ifndef KHASHMAP_H_
#define KHASHMAP_H_
#include <kstd/kstd.h>
typedef struct _khashmap{
    struct _ktree *root;    
}khashmap;
khashmap *khashmap_new();
void khashmap_free(khashmap *h);
void khashmap_debug_print(khashmap *h);
uint64_t khashmap_hash_string(char *str);
uint64_t khashmap_get(khashmap *h, char *str);
void khashmap_set(khashmap *h, char *k, uint64_t v);
#endif
