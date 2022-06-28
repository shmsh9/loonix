#include <drivers/serial.h>
#include <kstd.h>
uint8_t SERIAL_READCHAR(){
    SERIAL_WAITCHAR();
    return (uint8_t)inb(SERIAL_ADDRESS);
}
void SERIAL_WAITCHAR(){
    while( (inb(SERIAL_ADDRESS+5) & 1) == 0){
        kprintf("SERIAL_WAITCHAR() : inb == 0x%x\r", inb(SERIAL_ADDRESS+5));
    }
}
void SERIAL_PUTCHAR(char c){
    outb(SERIAL_ADDRESS, (uint32_t)c);
}

void SERIAL_INIT(){
   outb(SERIAL_ADDRESS, 0x00);    // Disable all interrupts

}
