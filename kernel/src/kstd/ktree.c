#include <kstd/ktree.h>

ktree *ktree_new(uint64_t p){
    ktree *ret = kmalloc(sizeof(ktree));
    *ret = (ktree){
        .payload = p,
        .left = NULL,
        .right = NULL,
        .root = NULL
    };
    return ret;
}
void ktree_debug_print(ktree *t, uint16_t level_count, char s){
    if(!t)
        return;
    kprintf("[%c][%d] 0x%x\n", 
        s, 
        level_count, 
        t->payload
    );
    ktree *l = t->left;
    ktree *r = t->right;
    if(l)
        ktree_debug_print(l, level_count+1, 'L');
    if(r)
        ktree_debug_print(r, level_count+1, 'R');
}
void ktree_add(ktree *t, uint64_t p){
    if(p > t->payload){
        //printf("add_tree(0x%llx, %lld) R\n", (uint64_t)t, p);
        if(!t->right){
            t->right = ktree_new(p);
            t->right->root = t;
            return;
        }
        ktree_add(t->right, p);
    }
    else{
        //printf("add_tree(0x%llx, %lld) L\n", (uint64_t)t, p);
        if(!t->left){
            t->left = ktree_new(p);
            t->left->root = t;
            return;
        }
        ktree_add(t->left, p);
    }
}
ktree *ktree_search(ktree *t, uint64_t p){
    //kprintf("ktree_search(0x%x, 0x%x)\n", (uint64_t)t, p);
    if(t->payload == p)
        return t;
    if(p > t->payload){
        if(!t->right)
            return NULL;
        return ktree_search(t->right, p);
    }
    else{
        if(!t->left)
            return NULL;
        return ktree_search(t->left, p);
    }
}
/*
void ktree_pop(ktree *t){
    if(!t->left && !t->right){
        ktree_free(t);
        return;
    }
    if(t->left && t->right){
        ktree *new_root = t->left;
    }
}
*/
void ktree_free(ktree *t){
    if(!t)
        return;
    ktree *l = t->left;
    ktree *r = t->right;
    kfree(t);
    ktree_free(l);
    ktree_free(r);
}
