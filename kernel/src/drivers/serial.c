#include <drivers/serial.h>

serial_device serial_device_new(){
    serial_device ret = (serial_device){
        .data = SERIAL_ADDRESS,
        .rx_mask = SERIAL_RX_MASK,
        .tx_mask = SERIAL_TX_MASK,
        .rx_tx = SERIAL_ADDRESS+SERIAL_RX_TX_OFFSET,
        .arch = ARCH_UINT
    };
    serial_device_init(&ret);
    return ret;
}
void serial_device_init(serial_device *serial){
    outb(serial->data, 0x00);
}
uint8_t serial_device_readchar(serial_device *serial){
    serial_device_waitrx(serial);
    uint8_t ret = (uint8_t)inb(serial->data);
    return ret;
}
void serial_device_waittx(serial_device *serial){
    switch(serial->arch){
        case ARCH_X64:
            while( (inb(serial->rx_tx) & serial->tx_mask) == 0){}
            break;
        case ARCH_AARCH64:
            while( (inb(serial->rx_tx)) == 0 ){}
            break;
    }
}
void serial_device_waitrx(serial_device *serial){
    switch(serial->arch){
        case ARCH_X64:
            while( (inb(serial->rx_tx) & serial->rx_mask) == 0){}
            break;
        case ARCH_AARCH64:
            while((inb(serial->rx_tx) & serial->rx_mask)){}
            break;
    }
}
void serial_device_putchar(serial_device *serial, uint8_t c){
    serial_device_waittx(serial);
    outb(serial->data, (uint32_t)c);
}
