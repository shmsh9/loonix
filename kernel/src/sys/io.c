#include <sys/io.h>

void outb(uint64_t address, uint32_t data){
    _OUTB(address, data);
}
void outw(uint64_t address, uint32_t data){
    _OUTW(address, data);
}
void outl(uint64_t address, uint32_t data){
    _OUTL(address, data);
}
uint32_t inb(uint64_t address){
    uint32_t r = 0;
    _INB(address, r);
    return r;
}
uint16_t inw(uint64_t address){
    uint16_t r = 0;
    _INW(address, r);
    return r;
}
uint32_t inl(uint64_t address){
    uint32_t r = 0;
    _INL(address, r);
    return r;
}
