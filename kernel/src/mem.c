#include <mem.h>
/*
		2014 Leonard Kevin McGuire Jr (www.kmcg3413.net) (kmcg3413@gmail.com)
		https://wiki.osdev.org/User:Pancakes/EntryBasedHeapImplementation
*/
void k_heapLCABInit(KHEAPLCAB *heap) {
		heap->fblock = 0;
		heap->bcnt = 0;
}

int k_heapLCABAddBlock(KHEAPLCAB *heap, uintptr_t addr, uint32_t size) {
	KHEAPBLOCKLCAB			*hb;
	KHEAPHDRLCAB			*hdr;
	
	//kprintf("add block addr:%p size:%x\n", addr, size);
	
	hb = (KHEAPBLOCKLCAB*)addr;
	hb->size = size;
	hb->used = 0;
	hb->next = heap->fblock;
	heap->fblock = hb;
	
	hdr = (KHEAPHDRLCAB*)&hb[1];
	hdr->flagsize = hb->size - (sizeof(KHEAPBLOCKLCAB) + 32);
	
	++heap->bcnt;
	
	hb->lastdsize = 0;
	hb->lastdhdr = 0;
	
	//kprintf("added block; block-count:%u\n", heap->bcnt);
	
	//kprintf("added block hb:%p hb->next:%p\n", hb, hb->next);
	//sleep(10);
	return 1;
}

/*
	Look behind and forward to see if we can merge back into some chunks.
*/
void k_heapLCABFree(KHEAPLCAB *heap, void *ptr) {
	KHEAPHDRLCAB				*hdr, *phdr, *nhdr;
	KHEAPBLOCKLCAB				*hb;
	//uint32_t						sz;
	//uint8_t						fg;
	
	//kprintf("lab pre-free ptr:%p\n", ptr);
	
	hdr = (KHEAPHDRLCAB*)ptr;
	//kprintf("GGGG\n");
	hdr[-1].flagsize &= ~0x80000000;
		
	//kprintf("lcab free\n");
		
	phdr = 0;
	/* find the block we are located in */
	for (hb = heap->fblock; hb; hb = hb->next) {
		//kprintf("lcab free looking at block:%p next:%p ptr:%p end:%p\n", hb, hb->next, ptr, (uintptr_t)hb + hb->size);
		if (((uintptr_t)ptr > (uintptr_t)hb) && ((uintptr_t)ptr < (uintptr_t)hb + hb->size)) {
			/* we have found the heap block */
			
			/* get header */
			hdr = (KHEAPHDRLCAB*)((uintptr_t)ptr - sizeof(KHEAPHDRLCAB));
			
			/* set to free */
			hdr->flagsize &= ~0x80000000;
			
			hb->used -= hdr->flagsize;
			
			/* get previous header if it exists */
			if (hdr->prevsize) {
				phdr = (KHEAPHDRLCAB*)((uintptr_t)&hdr - (sizeof(KHEAPHDRLCAB) + hdr->prevsize));
			} else {
				phdr = 0;
			}
			
			//kprintf("hdr:%p hdr->flagsize:%x hdr->prevsize:%x\n", hdr, hdr->flagsize, hdr->prevsize);
			/* get next header */
			nhdr = (KHEAPHDRLCAB*)((uintptr_t)&hdr[1] + hdr->flagsize);
			if ((uintptr_t)nhdr >= ((uintptr_t)hb + hb->size)) {
				nhdr = 0;
			}
						
			//fg = hdr->flagsize >> 31;
			//sz = hdr->flagsize & 0x7fffffff;			
			
			//kprintf("hdr:%p phdr:%p nhdr:%p phdr->flagsize:%x hdr->flagsize:%x\n", hdr, phdr, nhdr, phdr->flagsize, hdr->flagsize);
			if (nhdr) {
				if (!(nhdr->flagsize & 0x80000000)) {
					/* combine with it */
					hdr->flagsize += sizeof(KHEAPHDRLCAB) + nhdr->flagsize;
					hb->used -= sizeof(KHEAPHDRLCAB);
					/* set next header prevsize */
					nhdr = (KHEAPHDRLCAB*)((uintptr_t)&hdr[1] + hdr->flagsize);
					nhdr->prevsize = hdr->flagsize;
				}
			}
			
			//kprintf("here hdr:%p prevsize:%x\n", hdr, hdr->prevsize);
			
			/* can we combine with previous header? */
			if (phdr) {				
				if (!(phdr->flagsize & 0x80000000)) {
					//kprintf("combine backward\n");
					phdr->flagsize += sizeof(KHEAPHDRLCAB) + hdr->flagsize;
					hb->used -= sizeof(KHEAPHDRLCAB);
					hdr = phdr;
					/* set next header prevsize */
					nhdr = (KHEAPHDRLCAB*)((uintptr_t)&hdr[1] + hdr->flagsize);
					if ((uintptr_t)nhdr < (uintptr_t)hb + sizeof(KHEAPBLOCKLCAB) + hb->size) {
						nhdr->prevsize = hdr->flagsize;
					}
				}
			}
			
			/* optimization */
			if (hdr->flagsize > hb->lastdsize) {
				hb->lastdsize = hdr->flagsize;
				hb->lastdhdr = hdr;
			}
			
			return;
		}
	}
	
	kprintf("uhoh ptr: 0x%x\n", ptr);
	for (hb = heap->fblock; hb; hb = hb->next) {
		kprintf("lcab free looking at block: 0x%x next: 0x%x ptr: 0x%x end: 0x%x\n", hb, hb->next, ptr, (uintptr_t)hb + hb->size);
		if (((uintptr_t)ptr > (uintptr_t)hb)) {
			kprintf("above\n");
			if (((uintptr_t)ptr < ((uintptr_t)hb + hb->size))) {
				kprintf("found\n");
			}
		}
	}
	for (;;);
	/* uhoh.. this should not happen.. */
	return;
}

/*
	This will allocate a chunk of memory by the specified size, and if
	no memory chunk can be found it will return zero.
*/
void* k_heapLCABAlloc(KHEAPLCAB *heap, uint32_t size) {
	KHEAPBLOCKLCAB		*hb;
	KHEAPHDRLCAB		*hdr, *_hdr, *phdr;
	uint32_t				sz;
	uint8_t				fg;
	uint32_t				checks;
	uint32_t				bc;
	checks = 0;
	bc =0;
	
	//kprintf("lcab alloc\n");
	/* first find heap block with enough space */
	for (hb = heap->fblock; hb; hb = hb->next) {
		if ((hb->size - hb->used) >= (size + sizeof(KHEAPHDRLCAB))) {
			++bc;
			/* optimization */
			//if (hb->lastdsize >= size) {
				/* optimization (use block) */
				//hdr = hb->lastdhdr;
				//hb->lastdhdr = 0;
				//hb->lastdsize = 0;
			//} else {
			hdr = (KHEAPHDRLCAB*)&hb[1];
			//}
			
			//kprintf("enter loop\n");
			phdr = 0;
			while ((uintptr_t)hdr < ((uintptr_t)hb + hb->size)) {
				++checks;
				//kprintf("lcab alloc found\n");
				fg = hdr->flagsize >> 31;
				sz = hdr->flagsize & 0x7fffffff;
				//kprintf("hdr:%p fg:%x sz:%x\n", hdr, fg, sz);
				if (!fg) {
					//kprintf("lcab alloc got chunk\n");
					if (sz >= size) {
						//kprintf("lcab alloc thinking of splitting\n");
						// else take whole chunk
						if (sz > (size + sizeof(KHEAPHDRLCAB) + 16)) {
							//kprintf("lcab alloc splitting\n");
							/* has enough left over (break into two) */
							_hdr = (KHEAPHDRLCAB*)((uintptr_t)&hdr[1] + size);
							//kprintf("AA\n");
							/* take out data size and header size */
							_hdr->flagsize = sz - (size + sizeof(KHEAPHDRLCAB));
							_hdr->prevsize = size;
							//kprintf("BB\n");
							/* set to used and set new size */
							hdr->flagsize = 0x80000000 | size;
							//kprintf("CC\n");
							hb->used += sizeof(KHEAPHDRLCAB);
							//kprintf("DD\n");
						} else {
							/* simply set to used */
							hdr->flagsize |= 0x80000000;
						}
						hb->used += size;
						
						//kprintf("ptr:%p\n", &hdr[1]);
						//kprintf("alloced checks:%u blocks-checked:%u\n", checks, bc);
						
						return &hdr[1];
					}
				}
				phdr = hdr;
				hdr = (KHEAPHDRLCAB*)((uintptr_t)&hdr[1] + sz);
			}
			//kprintf("exit loop\n");
		}
	}
	
	//kprintf("return null\n");
	return 0;
}
