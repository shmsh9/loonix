#ifndef KTREE_H_
#define KTREE_H_
#include <stdint.h>
#include <kstd/kstd.h>

typedef struct _ktree{
   struct _ktree *root;
   struct _ktree *left;
   struct _ktree *right;
   uint64_t key;
   uint64_t value;
   int64_t lock;
}ktree;

ktree * ktree_new(uint64_t k, uint64_t v);
void ktree_debug_print(ktree *t, uint64_t l);
void ktree_add(ktree *t, uint64_t k, uint64_t v);
ktree *ktree_del(ktree *t, uint64_t k);
ktree * ktree_search(ktree *t, uint64_t k);
void ktree_free(ktree *t);
#endif
