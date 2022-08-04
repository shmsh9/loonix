#ifndef MEM_H_
#define MEM_H_
#include <stdint.h>
#include <kstd.h>


#define KHEAPFLAG_USED			0x80000000

typedef struct _KHEAPHDRLCAB {
	uint32_t				prevsize;
	uint32_t				flagsize;
} KHEAPHDRLCAB;

typedef struct _KHEAPBLOCKLCAB {
	uint32_t					size;
	uint32_t					used;
	struct _KHEAPBLOCKLCAB	                *next;
	uint32_t					lastdsize;
	KHEAPHDRLCAB			        *lastdhdr;
} KHEAPBLOCKLCAB;

typedef struct _KHEAPLCAB {
	KHEAPBLOCKLCAB		       *fblock;
	uint32_t				bcnt;
} KHEAPLCAB;

extern KHEAPLCAB HEAP;

void k_heapLCABInit(KHEAPLCAB *heap);
int k_heapLCABAddBlock(KHEAPLCAB *heap, uintptr_t addr, uint32_t size);
void k_heapLCABFree(KHEAPLCAB *heap, void *ptr);
void* k_heapLCABAlloc(KHEAPLCAB *heap, uint32_t size);

#endif
