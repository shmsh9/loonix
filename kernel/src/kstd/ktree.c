#include <kstd/ktree.h>
void ktree_lock(ktree *t){
    while(t->lock){
        sleep_100(1);
    }
    t->lock++;
}
void ktree_unlock(ktree *t){
    t->lock--;
}
ktree *ktree_new(uint64_t p){
    ktree *ret = kmalloc(sizeof(ktree));
    *ret = (ktree){
        .payload = p,
        .left = NULL,
        .right = NULL,
        .root = NULL,
        .lock = 0
    };
    return ret;
}
void ktree_debug_print(ktree *t, uint64_t l){
    if(!t)
        return;
    ktree_lock(t);
    for (int i = 0; i < l; i++)
        kprint(i == l - 1 ? "|-" : "  ");
    kprintf("0x%x\n", t->payload);
    ktree_debug_print(t->left, l+1);
    ktree_debug_print(t->right, l+1);    
    ktree_unlock(t);
}

void ktree_add(ktree *t, uint64_t p){
    if(!t)
        return;
    ktree_lock(t);
    if(p > t->payload){
        //printf("add_tree(0x%llx, %lld) R\n", (uint64_t)t, p);
        if(!t->right){
            t->right = ktree_new(p);
            t->right->root = t;
            ktree_unlock(t);
            return;
        }
        ktree_unlock(t);
        ktree_add(t->right, p);
    }
    else{
        //printf("add_tree(0x%llx, %lld) L\n", (uint64_t)t, p);
        if(!t->left){
            t->left = ktree_new(p);
            t->left->root = t;
            ktree_unlock(t);
            return;
        }
        ktree_unlock(t);
        ktree_add(t->left, p);
    }
}
ktree *ktree_search(ktree *t, uint64_t p){
    if(!t)
        return NULL;
    ktree_lock(t);
    //kprintf("ktree_search(0x%x, 0x%x)\n", (uint64_t)t, p);
    if(t->payload == p){
        ktree_unlock(t);
        return t;
    }
    if(p > t->payload){
        if(!t->right){
            ktree_unlock(t);
            return NULL;
        }
        ktree_unlock(t);
        return ktree_search(t->right, p);
    }
    else{
        if(!t->left){
            ktree_unlock(t);
            return NULL;
        }
        ktree_unlock(t);
        return ktree_search(t->left, p);
    }
}
ktree *ktree_min_payload(ktree *t){
    ktree *curr = t;
    while(curr && curr->left)
        curr = curr->left;
    return curr;
}
void ktree_free_node(ktree *t){
    if(!t)
        return;
    kfree(t);
}
ktree *ktree_del(ktree *t, uint64_t p){
    if(!t)
        return t;
    ktree_lock(t);
    if(p < t->payload)
        t->left = ktree_del(t->left, p);
    else if(p > t->payload)
        t->right = ktree_del(t->right, p);
    else{
        // node with only one child or no child
        if (t->left == NULL) {
            ktree* temp = t->right;
            ktree_free_node(t);
            return temp;
        }
        else if (t->right == NULL) {
            ktree* temp = t->left;
            ktree_free_node(t);
            return temp;
        }
        // node with two children:
        // Get the inorder successor
        // (smallest in the right subtree)
        ktree *temp = ktree_min_payload(t->right);
 
        // Copy the inorder
        // successor's content to this node
        t->payload = temp->payload;
 
        // Delete the inorder successor
        t->right = ktree_del(t->right, temp->payload);
    }
    ktree_unlock(t);
    return t;
}
void ktree_free(ktree *t){
    if(!t)
        return;
    ktree_lock(t);
    ktree *l = t->left;
    ktree *r = t->right;
    kfree(t);
    ktree_free(l);
    ktree_free(r);
}
