#ifndef NEW_MEM_H_
#define NEW_MEM_H_
#include <stdint.h>
#include <kstd.h>
#include <stdbool.h>
#define HEAP_BLOCK_SIZE  1024
#define HEAP_HEADER_SIZE HEAP_BLOCK_SIZE / 8

typedef struct _memblock {
    struct   _memblock *next;
    uint8_t  header[HEAP_HEADER_SIZE];
    uint8_t  block[HEAP_BLOCK_SIZE];
} memblock;

typedef struct _kheap {
    struct _memblock *root;
} kheap;
typedef struct _kheap_allocated_block{
    struct _memblock *block;
    uint16_t         bitfield;
    uint8_t          bit;
    uint32_t         size;
    uintptr_t        ptr;

}kheap_allocated_block;
bool get_bit(uint8_t field, uint8_t bit);
void set_bit(uint8_t *field, uint8_t bit);
void unset_bit(uint8_t *field, uint8_t bit);
void kheap_init(kheap *heap);
void kheap_add_block(kheap *heap, uintptr_t mem);
bool kheap_free_uint8(uint8_t header);
void kheap_set_used_bytes(struct _memblock *block, uint8_t start_bitfield, uint8_t start_bit, uint64_t size);
void kheap_unset_used_bytes(struct _memblock *block, uint8_t start_bitfield, uint8_t start_bit, uint64_t size);
void kheap_free_mem(kheap_allocated_block *k);
kheap_allocated_block kheap_get_free_mem(kheap *heap, uint64_t size);
void kheap_debug_print(kheap *heap);
#endif

