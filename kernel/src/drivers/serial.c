#include <drivers/serial.h>
#include <kstd.h>
uint8_t SERIAL_READCHAR(){
    SERIAL_WAITRX();
    uint8_t ret = (uint8_t)inb(SERIAL_ADDRESS);
    /*
    #ifdef __aarch64__
        //flush fifo
        outb(SERIAL_ADDRESS+0x02c, ret & (~(1 << 4)));
    #endif
    */
    return ret;
}
void SERIAL_WAITTX(){
    #ifdef __aarch664__
        uint32_t offset = 0x18;
        uint32_t mask = (1 << 3);
        while(! (inb(SERIAL_ADDRESS+offset) & mask ) ){}
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
void SERIAL_INIT(){
   #ifdef __x86_64__
    // Disable all interrupts
    outb(SERIAL_ADDRESS, 0x00);
   #endif
   #ifdef __aarch64__
    /*
    //credits to https://krinkinmu.github.io/2020/11/29/PL011.html
    //static const uint32_t DR_OFFSET = 0x000;
    //static const uint32_t FR_OFFSET = 0x018;
    static const uint32_t IBRD_OFFSET = 0x024;
    static const uint32_t FBRD_OFFSET = 0x028;
    static const uint32_t LCR_OFFSET = 0x02c;
    static const uint32_t CR_OFFSET = 0x030;
    static const uint32_t IMSC_OFFSET = 0x038;
    static const uint32_t DMACR_OFFSET = 0x048;
    static const uint32_t CR_TXEN = (1 << 8);
    static const uint32_t CR_UARTEN = (1 << 0);
    static const uint32_t LCR_FEN = (1 << 4);
    //static const uint32_t LCR_STP2 = (1 << 3);

    uint32_t cr = inb(SERIAL_ADDRESS+CR_OFFSET);
    uint32_t lcr = inb(SERIAL_ADDRESS+LCR_OFFSET);

    // Disable UART before anything else
    outb(SERIAL_ADDRESS+CR_OFFSET, (cr & CR_UARTEN));

    // Wait for any ongoing transmissions to complete
    //wait_tx_complete(dev);

    // Flush FIFOs
    outb(SERIAL_ADDRESS+LCR_OFFSET, (lcr & ~LCR_FEN));

    // Set frequency divisors (UARTIBRD and UARTFBRD) to configure the speed
    //claculate_divisors(dev, &ibrd, &fbrd);
    outb(SERIAL_ADDRESS+IBRD_OFFSET, (0x341 & 0x3f));
    outb(SERIAL_ADDRESS+FBRD_OFFSET, ((0x341 >> 6) & 0xffff) );

    // Mask all interrupts by setting corresponding bits to 1
    outb(SERIAL_ADDRESS+IMSC_OFFSET, 0x7ff);

    // Disable DMA by setting all bits to 0
    outb(SERIAL_ADDRESS+DMACR_OFFSET, 0x0);

    // I only need transmission, so that's the only thing I enabled.
    // *reg(dev, CR_OFFSET) = CR_TXEN;
    outb(SERIAL_ADDRESS+CR_OFFSET, CR_TXEN);
    // Finally enable UART
    outb(SERIAL_ADDRESS+CR_OFFSET, CR_TXEN | CR_UARTEN);
   */
    #endif
}
