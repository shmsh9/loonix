#include "stack.h"


void pushstack(struct stack *stack, void *data){
	struct node *n = malloc(sizeof(struct node));
	n->next = stack->root;
	stack->root = n;
}
void cleanstack(struct stack *stack){
	struct node *n = stack->root;
	if(!n->next){
		free(n->data);
		free(n);
	}
	while(n->next){
		struct node *next = n->next;
		free(n->data);
		free(n);
		n = next;
	}
}
