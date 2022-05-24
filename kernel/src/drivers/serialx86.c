#include <drivers/serialx86.h>

struct serial_port serialx86_new(uint16_t port){
    struct serial_port serial;
    //need a malloc before this
    serial.port    = port;
    serial.putchar = serialx86_putchar;
    serial.getchar = serialx86_getchar;
    serial.init    = serialx86_init;
    return serial;
}

void serialx86_putchar(struct serial_port serial, char b){
#ifdef __x86_64__
    uint16_t port = serial.port;
    __asm__ __volatile__ ("outb %0, %1" : : "a"(b), "Nd"(port));
#endif 
}
char serialx86_getchar(struct serial_port serial){
char ret = 0;
#ifdef __x86_64__
    uint16_t port = serial.port;
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
#endif
	return ret;
}

void __x86_outb(uint16_t port, uint8_t b){
#ifdef __x86_64__
    __asm__ __volatile__ ("outb %0, %1" : : "a"(b), "Nd"(port));
#endif 
}
uint8_t __x86_inb(uint16_t port){
    uint8_t ret = 0;
#ifdef __x86_64__
    __asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
#endif
	return ret;
}
int serialx86_init(struct serial_port serial){
    uint16_t port = serial.port;
    //uint16_t port = sx86.port;
    __x86_outb(port + 1, 0x00);    // Disable all interrupts
    __x86_outb(port + 3, 0x80);    // Enable DLAB (set baud rate divisor)
    __x86_outb(port + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
    __x86_outb(port + 1, 0x00);    //                  (hi byte)
    __x86_outb(port + 3, 0x03);    // 8 bits, no parity, one stop bit
    __x86_outb(port + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
    __x86_outb(port + 4, 0x0B);    // IRQs enabled, RTS/DSR set
    __x86_outb(port + 4, 0x1E);    // Set in loopback mode, test the serial chip
    __x86_outb(port + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
   // Check if serial is faulty (i.e: not same byte as sent)
   if(__x86_inb(port + 0) != 0xAE)
      return 1;
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   __x86_outb(port + 4, 0x0F);
   return 0;
}
