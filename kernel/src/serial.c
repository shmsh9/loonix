#include <serial.h>

int init_serial() {
#ifdef __x86_64__
   outb(SERIAL_PORT + 1, 0x00);    // Disable all interrupts
   outb(SERIAL_PORT + 3, 0x80);    // Enable DLAB (set baud rate divisor)
   outb(SERIAL_PORT + 0, 0x03);    // Set divisor to 3 (lo byte) 38400 baud
   outb(SERIAL_PORT + 1, 0x00);    //                  (hi byte)
   outb(SERIAL_PORT + 3, 0x03);    // 8 bits, no parity, one stop bit
   outb(SERIAL_PORT + 2, 0xC7);    // Enable FIFO, clear them, with 14-byte threshold
   outb(SERIAL_PORT + 4, 0x0B);    // IRQs enabled, RTS/DSR set
   outb(SERIAL_PORT + 4, 0x1E);    // Set in loopback mode, test the serial chip
   outb(SERIAL_PORT + 0, 0xAE);    // Test serial chip (send byte 0xAE and check if serial returns same byte)
 
   // Check if serial is faulty (i.e: not same byte as sent)
   if(inb(SERIAL_PORT + 0) != 0xAE) {
      return 1;
   }
 
   // If serial is not faulty set it in normal operation mode
   // (not-loopback with IRQs enabled and OUT#1 and OUT#2 bits enabled)
   outb(SERIAL_PORT + 4, 0x0F);
   return 0
#endif
#ifdef __aarch64__
	return pl011_setup(&SERIAL_PORT, 0x90000000, 24000000);
#endif
}
void outb(uint8_t val){
	#ifdef __x86_64__
	__asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(SERIAL_PORT));
	#endif
	#ifdef __aarch64__
	pl011_send(&SERIAL_PORT, val);
	#endif
}
uint8_t inb(){
	uint8_t ret = 0;
	#ifdef __x86_64__
	__asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(SERIAL_PORT));
	#endif
	#ifdef __aarch64__
	#endif
	return ret;
}
void wait_transmission(){
	#ifdef __x86_64__
	while(inb(SERIAL_PORT + 5) & 0x20);
	#endif
	#ifdef __aarch64__
	wait_tx_complete(&SERIAL_PORT);
	#endif	
}
void write_serial(char a) {
	wait_transmission();
	outb(a);
}
void puts_serial(const char *str){
    while(*str){
        write_serial(*str);
        str++;
    }
}
