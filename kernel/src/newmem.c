#include <newmem.h>

void kheap_init(kheap *heap){
    heap->memory = 0x0;
    heap->header = 0x0;
    heap->n_block = 0;
}
void kheap_add_blocks(kheap *heap, uintptr_t mem, uint64_t nblock){
    heap->n_block = nblock - ((HEAP_HEADER_SIZE*nblock)/HEAP_BLOCK_SIZE);
    KDEBUG("using %d blocks for header",  ((HEAP_HEADER_SIZE*nblock)/HEAP_BLOCK_SIZE));
    KDEBUG("adding %d MB at 0x%x", BYTES_TO_MB(heap->n_block*HEAP_BLOCK_SIZE), mem);
    uintptr_t heap_header_address = mem;
    uintptr_t heap_memory_address = heap_header_address+(HEAP_HEADER_SIZE*nblock);
    heap->header = (uint8_t *)heap_header_address;
    heap->memory = (uint8_t *)heap_memory_address;
    memset(heap->header, 0, HEAP_HEADER_SIZE*nblock);
    KDEBUG("heap->header : 0x%x heap->memory : 0x%x, heap->n_block : %d", heap->header, heap->memory, heap->n_block);
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
void kheap_set_used_bytes2(kheap *heap, uint64_t start_bitfield, uint8_t start_bit, uint64_t size){
    uint64_t header_size = HEAP_HEADER_SIZE*heap->n_block;
    uint64_t n_bit_set = 0;
    for(int i = start_bitfield; i < header_size; i++){
        uint8_t j = 0;
        switch(n_bit_set){
            case 0:
                j = start_bit;
                break;
            default:
                j = 0;
                break;
        }
        for(; j < 8; j++){
            set_bit(heap->header+i, j);
            n_bit_set++;
            if(n_bit_set == size)
                return;
        }
    }
}
void kheap_unset_used_bytes2(kheap *heap, uint64_t start_bitfield, uint8_t start_bit, uint64_t size){
    uint64_t n_bit_unset = 0;
    uint64_t header_size = HEAP_HEADER_SIZE*heap->n_block;

    for(int i = start_bitfield; i < header_size; i++){
        uint8_t j = 0;
        switch (n_bit_unset)
        {
        case 0:
            j = start_bit;
            break;
        default:
            j = 0;
            break;
        }
        for(; j < 8; j++){
            unset_bit(heap->header+i, j);
            n_bit_unset++;
            if(n_bit_unset == size)
                return;
        }
    }
}
void kheap_free_mem2(kheap *heap, kheap_allocated_block *k){
    kheap_unset_used_bytes2(heap, k->bitfield, k->bit, k->size);
}
kheap_allocated_block kheap_get_free_mem2(kheap *heap, uint64_t size){
    if(!heap->header || !heap->memory){
        KERROR("heap is not initialized");
        return  (kheap_allocated_block){0, 0, 0 ,0, 0};
    }
    uint64_t available_mem = heap->n_block * HEAP_BLOCK_SIZE;
    if(size > available_mem){
        KERROR("%d bytes exceeds total memory available (%d bytes)", size, available_mem);
        return  (kheap_allocated_block){0, 0, 0 ,0, 0};
    }
    uint64_t aligned_bytes = 0;
    uint64_t start_bitfield = 0;
    uint64_t start_bit = 0;
    uint64_t header_size = HEAP_HEADER_SIZE*heap->n_block;
    for(uint64_t bitfield = 0; bitfield < header_size; bitfield++){
        if(!kheap_free_uint8(heap->header[bitfield])){
            aligned_bytes = 0;
            start_bit = 0;
            start_bitfield = bitfield+1;
            continue;
        }
        for(uint8_t bitfield_byte = 0; bitfield_byte < 8; bitfield_byte++){
            //KDEBUG("testing heap->header[%d][%d]", bitfield, bitfield_byte);
            if(get_bit(heap->header[bitfield], bitfield_byte)){
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
        if(aligned_bytes == size){
            /*
            KDEBUG("found %d bytes free at block : 0x%x bitfield : %d bit : %d", 
            aligned_bytes, heap->memory+(start_bitfield*8)+start_bit,
            start_bitfield,
            start_bit);
            */
            kheap_set_used_bytes2(heap, start_bitfield, start_bit, aligned_bytes);
            return (kheap_allocated_block){
                .block = 0x0,
                .bitfield = start_bitfield,
                .bit = start_bit,
                .size = aligned_bytes,
                .ptr = (uintptr_t)heap->memory+(start_bitfield*8)+start_bit
            };
        }
    }
    KERROR("not enough free mem to allocate %d bytes", size);
    return  (kheap_allocated_block){0, 0, 0 ,0, 0};

}
void kheap_debug_print2(kheap *heap){
    for(uint64_t i = 0 ; i < heap->n_block; i++){
        KDEBUG("block 0x%x :", heap->memory+(i*HEAP_BLOCK_SIZE));
        for(uint64_t current_bitfield = 0; current_bitfield < HEAP_HEADER_SIZE; current_bitfield++){
            kprintf("[%d", current_bitfield);
            if(current_bitfield < 10){
                kprint("  ]");
            }
            if(current_bitfield >= 10 && current_bitfield < 100){
                kprint(" ]");
            }
            if(current_bitfield >= 100){
                kprint("]");
            }
            kprint(" [ ");
            for(uint8_t j = 0; j < 8; j++){
                kprintf("%d", get_bit(heap->header[current_bitfield+i*HEAP_HEADER_SIZE], j));
            }
            kprint(" ] ");
            if(!((current_bitfield+1) % 4) && current_bitfield != 0)
                kputc('\n');
            if(current_bitfield+1 == HEAP_HEADER_SIZE)
                kputc('\n');

        }
    }

}

void mmap_debug_print(mmap *mmap){
    char *efi_memory_types[16] = {0};
    efi_memory_types[0]  = "EFI_RESERVED_MEMORY_TYPE";
    efi_memory_types[1]  = "EFI_LOADER_CODE";
    efi_memory_types[2]  = "EFI_LOADER_DATA";
    efi_memory_types[3]  = "EFI_BOOT_SERVICES_CODE";
    efi_memory_types[4]  = "EFI_BOOT_SERVICES_DATA";
    efi_memory_types[5]  = "EFI_RUNTIME_SERVICES_CODE";
    efi_memory_types[6]  = "EFI_RUNTIME_SERVICES_DATA";
    efi_memory_types[7]  = "EFI_CONVENTIONAL_MEMORY";
    efi_memory_types[8]  = "EFI_UNUSABLE_MEMORY";
    efi_memory_types[9]  = "EFI_ACPI_RECLAIM_MEMORY";
    efi_memory_types[10] = "EFI_ACPI_MEMORY_NVS";
    efi_memory_types[11] = "EFI_MEMORY_MAPPED_IO";
    efi_memory_types[12] = "EFI_MEMORY_MAPPED_IO_PORT_SPACE";
    efi_memory_types[13] = "EFI_PAL_CODE";
    efi_memory_types[14] = "EFI_PERSISTENT_MEMORY";
    efi_memory_types[15] = "EFI_MAX_MEMORY_TYPE";
    uint8_t efi_memory_types_count = sizeof(efi_memory_types)/sizeof(efi_memory_types[0]);
    KDEBUG("mmap at 0x%x", mmap->mmap);
    KDEBUG("mmap length %d", mmap->length);
    for(int i = 0; i < mmap->length; i++){
        struct efi_memory_descriptor *desc = (struct efi_memory_descriptor *)((uint64_t)mmap->mmap+(i*MMAP_ELEMENT_SIZE));
        KDEBUG("mmap[%d]",i);
        KDEBUG("0x%x (%d KB)", 
                desc->physical_start, 
                BYTES_TO_KB(desc->pages*HEAP_BLOCK_SIZE)
                );
        KDEBUG("attributes 0x%x", desc->attributes);
        KDEBUG("%s", 
                desc->type < efi_memory_types_count ? 
                    efi_memory_types[desc->type] : "TYPE_NOT_FOUND"
                );
    } 
}

mmap mmap_new(struct bootinfo *bootinfo){
    mmap ret = (mmap){0};
    if(!bootinfo->mmap){
        KERROR("mmap == 0x0");
        return ret;
    }
    if(!bootinfo->mmap_size){
        KERROR("mmap is empty");
        return ret;
    }
    ret.mmap = bootinfo->mmap;
    ret.length = bootinfo->mmap_size/MMAP_ELEMENT_SIZE;
    return ret;
}

struct efi_memory_descriptor * mmap_find_largest_block(mmap *mmap){
    if(!mmap){
        KERROR("mmap == NULL");
        return 0x0;
    }
    uint64_t largest_mem = 0;
    struct efi_memory_descriptor *ret = 0;
    for(int i = 0; i < mmap->length; i++){
        struct efi_memory_descriptor *desc = (struct efi_memory_descriptor *)((uint64_t)mmap->mmap+(i*MMAP_ELEMENT_SIZE));
        if(desc->type == EFI_CONVENTIAL_MEMORY && desc->pages > largest_mem){
            ret = desc;
            largest_mem = desc->pages;
        }
    } 
   return ret; 
}

