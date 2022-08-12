#ifndef NEW_MEM_H_
#define NEW_MEM_H_
#include <stdint.h>
#include <stdbool.h>

#ifdef __x86_64__
	#define ALIGN 0x1000
#endif
#ifdef __aarch64__
	#define ALIGN 0x10
#endif
#define HEAP_BLOCK_SIZE  4096
#define HEAP_HEADER_SIZE HEAP_BLOCK_SIZE/8
#define HEAP_BLOCK_NUMBER 2

typedef struct{
    uint8_t block[HEAP_BLOCK_SIZE];
}block;
typedef struct _memblock {
    uint8_t  header[HEAP_HEADER_SIZE];
    uint8_t  block[HEAP_BLOCK_SIZE];
    struct   _memblock *next;
} memblock;

typedef struct _memblock_header {
    uint8_t header[HEAP_HEADER_SIZE*HEAP_BLOCK_NUMBER];
}memblock_header;

typedef struct _kheap {
    struct _memblock *root;
    uint8_t *header;
    uint8_t  *memory;
    uint16_t n_block;
} kheap;

typedef struct _kheap_allocated_block{
    struct _memblock *block;
    uint16_t         bitfield;
    uint8_t          bit;
    uint32_t         size;
    uintptr_t        ptr;

}kheap_allocated_block;
#include <kstd.h>

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

