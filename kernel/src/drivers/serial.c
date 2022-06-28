#include <drivers/serial.h>
#include <kstd.h>
uint8_t SERIAL_READCHAR(){
    SERIAL_WAITCHAR();
    return (uint8_t)inb(SERIAL_ADDRESS);
}
void SERIAL_WAITCHAR(){
    int offset = 0;
    #ifdef __aarch664__
        offset = 0x018;
    #endif
    #ifdef __x86_64__
        offset = 0x05
    #endif
    while( (inb(SERIAL_ADDRESS+offset) & 1) == 0){
        kprintf("SERIAL_WAITCHAR() : inb == 0x%x\r", inb(SERIAL_ADDRESS+5));
    }
}
void SERIAL_PUTCHAR(char c){
    outb(SERIAL_ADDRESS, (uint32_t)c);
}

void SERIAL_INIT(){
   outb(SERIAL_ADDRESS, 0x00);    // Disable all interrupts

}
