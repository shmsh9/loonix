#include <drivers/serial.h>

void serial_device_init(serial_device *serial){
    if(!serial)
        return;
    outb(serial->data, 0x00);
    if(inb(serial->data) >= 0xff){
        KERROR("serial is sending garbage disabling it");
        memset(serial, 0, sizeof(serial_device));
    }
}
uint8_t serial_device_readchar(serial_device *serial){
    if(!serial)
        return 0;
    serial_device_waitrx(serial);
    uint8_t ret = (uint8_t)inb(serial->data);
    return ret;
}
uint8_t serial_device_readchar_non_blocking(serial_device *serial){
    if(!serial)
        return 0;
    return serial->getchar_non_blocking(serial);
}
void serial_device_waittx(serial_device *serial){
    if(!serial)
        return;
    serial->wait_tx(serial);
}
void serial_device_waitrx(serial_device *serial){
    if(!serial)
        return;
    serial->wait_rx(serial);
}
void serial_device_putchar(serial_device *serial, uint8_t c){
    if(!serial)
        return;
    serial->wait_tx(serial);
    outb(serial->data, (uint32_t)c);
}

