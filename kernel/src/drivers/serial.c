#include <drivers/serial.h>
#include <kstd.h>
uint8_t SERIAL_READCHAR(){
    SERIAL_WAITRX();
    uint8_t ret = (uint8_t)inb(SERIAL_ADDRESS);
    return ret;
}
void SERIAL_WAITTX(){
    #ifdef __aarch64__
        uint32_t offset = 0x18;
        uint32_t mask = (1 << 5);
        while((inb(SERIAL_ADDRESS+offset) & mask )){}
    #endif
    #ifdef __x86_64__
        uint32_t offset = 0x5;
        uint32_t mask   = 0x20;
        while( (inb(SERIAL_ADDRESS+offset) & mask) == 0 ){ }
    #endif
}
void SERIAL_WAITRX(){
    #ifdef __aarch64__
        uint32_t offset = 0x18;
				uint32_t mask = (1 << 4);
        while( (inb(SERIAL_ADDRESS+offset) & mask) ){ }
    #endif
    #ifdef __x86_64__
        uint32_t offset = 0x5;
        uint32_t mask   = 0x1;
        while( (inb(SERIAL_ADDRESS+offset) & mask) == 0 ){ }
    #endif

}
void SERIAL_PUTCHAR(uint8_t c){
    SERIAL_WAITTX();
    outb(SERIAL_ADDRESS, (uint32_t)c);
}
