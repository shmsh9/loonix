#include <kstd/khashmap.h>

khashmap *khashmap_new(){
    khashmap *ret = kmalloc(sizeof(khashmap));
    *ret = (khashmap){
        .root = ktree_new(UINT64_MAX>>1, 0x0)
    };
    return ret;
}
void khashmap_free(khashmap *h){
    ktree_free(h->root);
    kfree(h);
}
void khashmap_debug_print(khashmap *h){
    ktree_debug_print(h->root, 0);
}
uint64_t khashmap_hash_string(char *str){
    uint64_t l = strlen(str);
    uint8_t h[8] = {0};
    int j = 0;
    for(int i = 0; i < l; i++){
        if(j == 8)
            j = 0;
        h[j] ^= str[i]+i;
        j++;
    }
    return *((uint64_t *)(h));
}
uint64_t khashmap_get(khashmap *h, char *str){
    if(!h)
        return 0x0;
    if(!str)
        return 0x0;
    ktree *t = ktree_search(h->root, khashmap_hash_string(str));
    if(!t)
        return 0x0;
    return t->value;
}
void khashmap_set(khashmap *h, char *k, uint64_t v){
    if(!h)
        return;
    uint64_t hstr = khashmap_hash_string(k);
    ktree *t = ktree_search(h->root, hstr);
    if(t){
        t->value = v;
        return;
    }
    ktree_add(h->root, hstr, v);
}
