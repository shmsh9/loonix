#include <io.h>
uint32_t inb(uint64_t address){
    uint32_t r = 0;
    _INB(address, r);
    return r;
}

void outb(uint64_t address, uint32_t data){
    _OUTB(address, data);
}
