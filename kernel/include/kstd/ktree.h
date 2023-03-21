#ifndef KTREE_H_
#define KTREE_H_
#include <stdint.h>
#include <kstd/kstd.h>

typedef struct _ktree{
   struct _ktree *root;
   struct _ktree *left;
   struct _ktree *right;
    uint64_t payload;
}ktree;

ktree * ktree_new(uint64_t p);
void ktree_debug_print(ktree *t, uint16_t level_count, char s);
void ktree_add(ktree *t, uint64_t p);
ktree * ktree_search(ktree *t, uint64_t p);
void ktree_free(ktree *t);
#endif
