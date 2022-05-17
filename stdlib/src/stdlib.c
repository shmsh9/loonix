#include <stdlib_h.h>
void *malloc(size_t sz){
	void *r = 0x0;
	uefi_call_wrapper(gBS->AllocatePool,3,EfiLoaderData, sz, &r);
	return r;
}
void *calloc(size_t elementCount, size_t elementSize){
	void *r = malloc(elementCount*elementSize);
	SetMem(r, elementCount*elementSize, 0);
	return r;
}
