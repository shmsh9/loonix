#include <serial.h>
#ifdef __x86_64__
void outb(uint16_t port, uint8_t val){
	__asm__ __volatile__ ("outb %0, %1" : : "a"(val), "Nd"(port));
}
uint8_t inb(uint16_t port){
	uint8_t ret = 0;
	__asm__ __volatile__ ("inb %1, %0" : "=a"(ret) : "Nd"(port));
	return ret;
}
int init_serial() {
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
   return 0;
}
#endif
#ifdef __aarch64__
void outb(uint64_t port, uint8_t val){
	// (1<<4) == full neet to wait
	while( (UART->channel_sts_reg0 & (1<<4)) != 0){}
	*SERIAL_FIFO = (uint32_t)val;
}
uint8_t inb(uint64_t port){
	return (uint8_t)*(uint32_t *)SERIAL_FIFO;
}
int init_serial(){
/*
	UART->control_reg0=0;
	UART->baud_rate_divider=6;
	UART->baud_rate_gen=62;
	UART->mode_reg0=(1<<5);
	UART->control_reg0=(1<<4) | (1<<2) | (1<<2) | (1<<0);
	*/
	outb(SERIAL_PORT, 0xAE);
	if(inb(SERIAL_PORT) != 0xAE)
		return 1;
	return 0;
}
#endif

int is_transmit_empty() {
   return inb(SERIAL_PORT + 5) & 0x20;
}
 
void write_serial(char a) {
   while (is_transmit_empty() == 0);
 
   outb(SERIAL_PORT,a);
}

void puts_serial(const char *str){
    while(*str){
        write_serial(*str);
        str++;
    }
}
