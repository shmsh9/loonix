//credits to https://krinkinmu.github.io/2020/11/29/PL011.html
#include <drivers/serialaa64.h>
#ifdef __aarch64__
struct serial_port serialaa64_new(){
    struct serial_port serial;
    serial.port = 0;
    serial.init = serialaa64_init;
    serial.putchar = serialaa64_putchar;
    serial.getchar = serialaa64_getchar;
    return serial;
}
int serialaa64_init(struct serial_port serial){
    uint32_t cr = __aarch64_getreg(serial.port+CR_OFFSET); 
    uint32_t lcr = __aarch64_getreg(serial.port+LCR_OFFSET);
    //disable uart
    __aarch64_setreg(serial.port+CR_OFFSET, (cr & CR_UARTEN));
    serialaa64_waittx(serial);
    //flush fifo
    __aarch64_setreg(serial.port+LCR_OFFSET, (lcr & ~LCR_FEN)); 
    uint32_t ibrd, fbrd;
    serialaa64_calculatedivisors(&ibrd, &fbrd);
    uint32_t databits = 8;
    lcr = 0x0;
    lcr |= ((databits - 1) & 0x3) << 5;
    __aarch64_setreg(serial.port+IMSC_OFFSET, 0x7ff);
    __aarch64_setreg(serial.port+DMACR_OFFSET, 0x0);
    //Only transmit
    __aarch64_setreg(serial.port+CR_OFFSET, CR_TXEN);
    __aarch64_setreg(serial.port+CR_OFFSET, CR_TXEN | CR_UARTEN);
    
    return 0;
}
char serialaa64_getchar(struct serial_port serial){
    serialaa64_waittx(serial);
    return *((char *)((void*)serial.port));
}
void serialaa64_calculatedivisors(uint32_t *integer, uint32_t *fractional){
    /*
    Fixme init fpu to get proper div
    
    uint32_t baudrate = 115200;
    uint32_t base_clock = 24000000;
    uint32_t div = 4 * base_clock / baudrate;
    */
    uint32_t div = 0x341;
    *fractional = div & 0x3f;
    *integer = (div >> 6) & 0xffff;
}
void serialaa64_putchar(struct serial_port serial, char b){
    serialaa64_waittx(serial);
    __aarch64_outb(serial, (uint32_t)b);
}
void serialaa64_waittx(struct serial_port serial){
   //while( __aarch64_getreg(serial.port+FR_OFFSET) * FR_BUSY != 0){} 
}
void __aarch64_outb(struct serial_port serial, uint32_t b){
    __aarch64_setreg(serial.port+CR_OFFSET, b);
}
volatile int __aarch64_inb(struct serial_port serial){
    return *(volatile uint32_t *)serial.port;
}
inline void __aarch64_setreg(uint64_t reg, uint32_t val){
    __asm__ __volatile__ (
        "str w1, [x0]"
    );
}
inline volatile uint32_t __aarch64_getreg(uint64_t reg){
    uint32_t ret = 0;
    __asm__ __volatile (
        "ldr w6, [x0]\t\n"
        "mov %w0, w6"
        : "=r"(ret)
    );
    return ret;
}
#endif

