#ifndef STACK_H_
#define STACK_H_
#include "std.h"
struct node{
	struct node *next;
	void 		*data;
};
struct stack {
	struct node *root;	
};
void pushstack(struct stack *stack, void *data);
void cleanstack(struct stack *stack);
#endif
