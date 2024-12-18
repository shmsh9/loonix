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

typedef enum {
    fs_entry_type_file,
    fs_entry_type_dir
} fs_entry_type;

typedef struct __attribute__((packed)) {
    fs_entry_type type;
    char name[256];
    uint64_t next_entry;
    void *data;
}fs_entry;

typedef struct __attribute__((packed)) {
    fs_entry entry;
    uint64_t first_child;
} fs_dir_entry;

typedef struct __attribute__((packed)) {
    fs_entry entry;
    uint64_t size;
    uint64_t first_block;
} fs_file_entry;

typedef struct __attribute__((packed)) {
    uint32_t magic;
    uint64_t block_n;
    uint16_t block_sz;
    fs_dir_entry root;
    PASTE_3(uint,FS_BITMAP_SZ,_t) *bitmap_blocks;
}fs_md;

#endif
