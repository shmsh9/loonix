#include <stdlib_h.h>
void *malloc(size_t sz){
	return (void*)SYSCALL(SYS_MALLOC, ((struct args){sz,0,0,0,0,0}));
}
void *calloc(size_t elementCount, size_t elementSize){
	void *r = malloc(elementCount*elementSize);
	if(r)
		memset(r, 0,elementCount*elementSize);
	return r;
}
void free(void *ptr){
	SYSCALL(SYS_FREE, ((struct args){(size_t)ptr,0,0,0,0,0}));
}
