#include <stack.h>


void pushstack(struct stack *stack, void *data){
	struct node *n = kmalloc(sizeof(struct node));
	n->data = data;
	n->prev = NULL;
	n->next = stack->root;
	if(stack->root)
		stack->root->prev = n;
	stack->root = n;
}
void cleanstack(struct stack *stack){
	struct node *n = stack->root;
	stack->root = NULL;
	if(n){
		if(!n->next){
			kfree(n->data);
			kfree(n);
			goto end;
		}
		while(n->next){
			struct node *next = n->next;
			kfree(n->data);
			kfree(n);
			n = next;
		}
	}
	end:;
}
