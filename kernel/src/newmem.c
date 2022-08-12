#include <newmem.h>

void kheap_init(kheap *heap){
    heap->root = 0x0;
}

void kheap_add_block(kheap *heap, uintptr_t mem){
    struct _memblock *m = (struct _memblock *)mem;
    m->next = 0;
    KDEBUG("adding block 0x%x of size %d bytes && header of %d bytes", mem+HEAP_HEADER_SIZE, HEAP_BLOCK_SIZE, HEAP_HEADER_SIZE);
    memset(m->header, 0, HEAP_HEADER_SIZE);
    if(!heap->root){
        heap->root = m;
        return;
    }
    struct _memblock *current = heap->root;
    while(current->next){
        current = current->next;
    }
    current->next = m;
}

bool get_bit(uint8_t field, uint8_t bit){
    return field >> bit & 0x1;
}
void set_bit(uint8_t *field, uint8_t bit){
    *field |= (1 << bit);
}
void unset_bit(uint8_t *field, uint8_t bit){
    *field &= ~(1 << bit);
}
bool kheap_free_uint8(uint8_t header){
    return (header & 0xff) != 0xff;
}
void kheap_set_used_bytes(struct _memblock *block, uint8_t start_bitfield, uint8_t start_bit, uint64_t size){
    struct _memblock *tmpblock = block;
    uint64_t n_bit_set = 0;
    while(n_bit_set != size){
        for(int i = start_bitfield; i < HEAP_HEADER_SIZE; i++){
            uint8_t j = n_bit_set == 0 ? start_bit : 0;
            for(; j < 8; j++){
                set_bit(tmpblock->header+i, j);
                n_bit_set++;
                if(n_bit_set == size)
                    return;
            }
        }
        tmpblock = tmpblock->next;
    }
}
void kheap_unset_used_bytes(struct _memblock *block, uint8_t start_bitfield, uint8_t start_bit, uint64_t size){
    uint64_t n_bit_unset = 0;
    for(int i = start_bitfield; i < HEAP_HEADER_SIZE; i++){
        uint8_t j = n_bit_unset == 0 ? start_bit : 0;
        for(; j < 8; j++){
            unset_bit(block->header+i, j);
            n_bit_unset++;
            if(n_bit_unset == size)
                return;
        }
    }
}
void kheap_free_mem(kheap_allocated_block *k){
    kheap_unset_used_bytes(k->block, k->bitfield, k->bit, k->size);
}
kheap_allocated_block kheap_get_free_mem(kheap *heap, uint64_t size){
    /*
    if(size > HEAP_BLOCK_SIZE){
        KERROR("size > HEAP_BLOCK_SIZE !");
        return  (kheap_allocated_block){0, 0, 0 ,0, 0};
    }
    */
    if(!heap->root){
        KERROR("heap is not initialized");
        return  (kheap_allocated_block){0, 0, 0 ,0, 0};
    }
    struct _memblock *current = heap->root;
    uint64_t aligned_bytes = 0;
    uint64_t start_bitfield = 0x0;
    uint64_t start_bit = 0x0;
    struct _memblock *start_block = heap->root;
    while(current){
        if(aligned_bytes == 0)
            start_block = current;
        for(uint64_t bitfield =  0; bitfield < HEAP_HEADER_SIZE; bitfield++){
            if(!kheap_free_uint8(current->header[bitfield])){
                start_bitfield = bitfield+1;
                start_bit = 0;
                aligned_bytes = 0;
                continue;
            }
            for(uint8_t bitfield_byte = 0; bitfield_byte < 8; bitfield_byte++){
                if(get_bit(current->header[bitfield], bitfield_byte)){
                    aligned_bytes = 0;
                    start_bitfield = bitfield;
                    start_bit = bitfield_byte + 1 < 8 ? bitfield_byte+1 : 0;
                }
                else{
                    aligned_bytes++;
                }
                if(aligned_bytes == size){
                    break;
                }
            }
            if(aligned_bytes == size)
                break;
        }
        if(aligned_bytes == size){
            KDEBUG("found %d bytes free at block : 0x%x bitfield : %d bit : %d", aligned_bytes, start_block,start_bitfield, start_bit);
            kheap_set_used_bytes(start_block, start_bitfield, start_bit, aligned_bytes);
            return (kheap_allocated_block){
                .block = start_block,
                .bitfield = start_bitfield,
                .bit = start_bit,
                .size = aligned_bytes,
                .ptr = (uintptr_t)start_block->block+(start_bitfield*8)+start_bit
            };
        }
        KDEBUG("not enough free mem changing block");
        current = current->next;
        start_bit = 0;
        start_bitfield = 0;
    }
    KERROR("not enough free blocks to allocate %d bytes !", size);
    return (kheap_allocated_block){0, 0, 0 ,0, 0};
}

void kheap_debug_print(kheap *heap){
    struct _memblock *current = heap->root;
    while(current){
        KDEBUG("block 0x%x : \n\n", current->block);
        for(uint64_t i = 0 ; i < HEAP_HEADER_SIZE; i++){
            kprintf("[%d", i);
            if(i < 10){
                kprint("  ]");
            }
            if(i >= 10 && i < 100){
                kprint(" ]");
            }
            if(i >= 100){
                kprint("]");
            }
            kprint(" [ ");
            for(uint8_t j = 0; j < 8; j++){
                kprintf("%d", get_bit(current->header[i], j));
            }
            kprint(" ] ");
            if(!((i+1) % 4) && i != 0)
                kputc('\n');
            if(i+1 == HEAP_HEADER_SIZE)
                kputc('\n');
        }
        current = current->next;
    }
}
