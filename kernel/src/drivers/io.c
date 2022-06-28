#include <io.h>
uint32_t inb(uint64_t address){
    uint32_t r = 0;
    #ifdef __x86_64__
        uint8_t tmp = 0;
        __asm__ __volatile__("inb %1, %0" : : "a"(tmp), "Nd"((uint16_t)address));
        r = (uint32_t)r;
    #endif 

    #ifdef __aarch64__
        r = *(volatile uint32_t *)address;
    #endif
    return r;
}

void outb(uint64_t address, uint32_t data){
    #ifdef __x86_64__
        __asm__ __volatile__("outb %0, %1" : : "a"((uint8_t)data), "Nd"((uint16_t)address));
    #endif 
    #ifdef __aarch64__
        *(volatile uint32_t *)address = data;
    #endif
}
