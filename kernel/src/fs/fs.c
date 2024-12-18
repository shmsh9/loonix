#include <fs/fs.h>

bool is_fs(uint8_t *b){
    return ((fs_md *)b)->magic == FS_MAGIC && ((fs_md *)b)->block_sz == FS_BLOCK_SZ;
}

bool set_block_dirty(fs_md *fs, uint64_t b){
    if(b >= fs->block_n)
        return false;
    task_lock();
    set_bit(fs->bitmap_blocks+(b/FS_BITMAP_SZ),  b%FS_BITMAP_SZ);
    task_unlock();
    return true;
}
bool set_block_clean(fs_md *fs, uint64_t b){
    if(b >= fs->block_n)
        return false;
    task_lock();
    unset_bit(fs->bitmap_blocks+(b/FS_BITMAP_SZ),  b%FS_BITMAP_SZ);
    task_unlock();
    return true;
}
