#ifndef NEW_MEM_H_
#define NEW_MEM_H_
#include <stdint.h>
#include <stdbool.h>
#include <bootloader.h>


#define HEAP_BLOCK_SIZE  4096
#define HEAP_HEADER_SIZE HEAP_BLOCK_SIZE/8
#define MMAP_ELEMENT_SIZE 48
typedef struct _task task;
typedef struct{
    uint8_t block[HEAP_BLOCK_SIZE];
}block;
typedef struct _memblock {
    uint8_t  header[HEAP_HEADER_SIZE];
    uint8_t  block[HEAP_BLOCK_SIZE];
    struct   _memblock *next;
} memblock;

typedef struct _kheap {
    uint8_t *header;
    uint8_t  *memory;
    uint64_t n_block;
    uint64_t free_memory;
} kheap;
typedef struct _kheap_allocated_block{
    struct _memblock *block;
    uint64_t         bitfield;
    uint8_t          bit;
    uint64_t         size;
    uintptr_t        ptr;
    task            *task;
}kheap_allocated_block;

typedef struct {
    EFI_MEMORY_DESCRIPTOR *mmap;
    uint64_t length;
}mmap;
#include <kstd/kstd.h>

bool get_bit(uint8_t field, uint8_t bit);
void set_bit(uint8_t *field, uint8_t bit);
void unset_bit(uint8_t *field, uint8_t bit);
void kheap_init(kheap *heap);
void kheap_add_blocks(kheap *heap, uintptr_t mem, uint64_t nblock);
bool kheap_free_uint8(uint8_t header);
void kheap_set_used_bytes2(kheap *heap, uint64_t start_bitfield, uint8_t start_bit, uint64_t size);
void kheap_unset_used_bytes2(kheap *heap, uint64_t start_bitfield, uint8_t start_bit, uint64_t size);
void kheap_free_mem2(kheap *heap, kheap_allocated_block *k);
kheap_allocated_block kheap_get_free_mem2(kheap *heap, uint64_t size);
void kheap_debug_print2(kheap *heap);
EFI_MEMORY_DESCRIPTOR * mmap_find_largest_block(mmap *mmap);
void mmap_debug_print(mmap *mmap);
mmap mmap_new(bootinfo *bootinfo);
#endif

