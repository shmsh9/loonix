#include <stdlib_h.h>
void *malloc(size_t sz){
	syscallargs->arg0 = sz;
	return (void*)SYSCALL(SYS_MALLOC);
}
void *calloc(size_t elementCount, size_t elementSize){
	void *r = malloc(elementCount*elementSize);
	if(r)
		memset(r, 0,elementCount*elementSize);
	return r;
}
void free(void *ptr){
	syscallargs->arg0 = (size_t)ptr;
	SYSCALL(SYS_FREE);
}
