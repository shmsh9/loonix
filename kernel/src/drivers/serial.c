#include <drivers/serial.h>

int serial_device_init(serial_device *serial){
    if(!serial)
        return -1;
    outb(serial->data, 0x00);
    if(inb(serial->data) >= 0xff){
        KERROR("serial replied garbage, disabling it");
        return -1;
    }
    return 0;
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

