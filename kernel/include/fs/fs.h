#ifndef FS_H_
#define FS_H_

#include <stdint.h>
#include <kstd/kstd.h>

#ifndef PASTE_3
#define _PASTE_3(A,B,C) A ## B ## C
#define PASTE_3(A,B,C) _PASTE_3(A,B,C)
#endif

#define FS_MAGIC 0xa1b2e5ff
#define FS_BLOCK_SZ 4096
#define FS_BITMAP_SZ 8

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint64_t block_n;
    uint16_t block_sz;
    PASTE_3(uint,FS_BITMAP_SZ,_t) *bitmap_blocks;
}fs_md;


#endif
